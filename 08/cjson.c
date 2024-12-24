#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
  Прототипы функций, которые будут экспортироваться:
  - cjson.loads  -> распарсить строку JSON и вернуть dict
  - cjson.dumps  -> взять dict и превратить в строку JSON
*/

static PyObject *cjson_loads(PyObject *self, PyObject *args);
static PyObject *cjson_dumps(PyObject *self, PyObject *args);

static const char *skip_whitespace(const char *str) {
    while (*str && isspace((unsigned char)*str)) {
        str++;
    }
    return str;
}

static const char *expect_char(const char *str, char c) {
    str = skip_whitespace(str);
    if (*str != c) {
        return NULL;
    }
    return ++str;
}

static const char *parse_string(const char *str, char **out) {
    str = skip_whitespace(str);
    if (*str != '\"') {
        return NULL;
    }
    str++;

    const char *start = str;
    while (*str && *str != '\"') {
        str++;
    }

    size_t length = str - start;
    char *temp = (char *)malloc(length + 1);
    if (!temp) {
        return NULL;
    }
    strncpy(temp, start, length);
    temp[length] = '\0';

    *out = temp;
    str++; // Пропускаем '"'
    return str;
}

static const char *parse_number(const char *str, long *out) {
    char *end = NULL;
    long val = strtol(str, &end, 10);
    if (str == end) {
        return NULL;
    }
    *out = val;
    return end;
}


// основной парсер

static PyObject *parse_json_object(const char *str) {
    // Ожидаем '{'
    str = skip_whitespace(str);
    if (!str || *str != '{') {
        return NULL;
    }
    str++;

    PyObject *dict = PyDict_New();
    if (!dict) {
        return NULL;
    }

    while (1) {
        str = skip_whitespace(str);
        if (!str) {
            Py_DECREF(dict);
            return NULL;
        }
        if (*str == '}') {
            str++;
            return dict;
        }

        char *key = NULL;
        str = parse_string(str, &key);
        if (!str || !key) {
            Py_DECREF(dict);
            return NULL;
        }

        str = expect_char(str, ':');
        if (!str) {
            free(key);
            Py_DECREF(dict);
            return NULL;
        }

        str = skip_whitespace(str);
        if (!str) {
            free(key);
            Py_DECREF(dict);
            return NULL;
        }

        PyObject *value_obj = NULL;

        if (*str == '\"') {
            // Строка
            char *val_str = NULL;
            str = parse_string(str, &val_str);
            if (!str || !val_str) {
                free(key);
                Py_XDECREF(value_obj);
                Py_DECREF(dict);
                return NULL;
            }
            value_obj = Py_BuildValue("s", val_str);
            free(val_str);
        } else if (isdigit((unsigned char)*str) || *str == '-') {
            long val_num;
            const char *new_pos = parse_number(str, &val_num);
            if (!new_pos) {
                free(key);
                Py_DECREF(dict);
                return NULL;
            }
            str = new_pos;
            value_obj = Py_BuildValue("l", val_num);
        } else {
            free(key);
            Py_DECREF(dict);
            return NULL;
        }

        if (PyDict_SetItemString(dict, key, value_obj) < 0) {
            free(key);
            Py_DECREF(value_obj);
            Py_DECREF(dict);
            return NULL;
        }

        free(key);
        Py_DECREF(value_obj);

        str = skip_whitespace(str);
        if (!str) {
            Py_DECREF(dict);
            return NULL;
        }

        if (*str == '}') {
            str++;
            return dict;
        }

        str = expect_char(str, ',');
        if (!str) {
            Py_DECREF(dict);
            return NULL;
        }
    }
}

static PyObject *cjson_loads(PyObject *self, PyObject *args) {
    const char *json_str = NULL;

    if (!PyArg_ParseTuple(args, "s", &json_str)) {
        PyErr_SetString(PyExc_TypeError, "Argument must be a string");
        return NULL;
    }

    // Парсим объект
    PyObject *result = parse_json_object(json_str);
    if (!result) {
        // Если парсер вернул NULL, выставим ошибку
        PyErr_SetString(PyExc_ValueError, "Invalid JSON format or parse error");
        return NULL;
    }

    return result;
}

static int cjson_dump_value(PyObject *value, FILE *out);

static int cjson_dump_dict(PyObject *dict, FILE *out) {
    fputc('{', out);

    PyObject *keys = PyDict_Keys(dict);
    if (!keys) {
        return -1;
    }
    Py_ssize_t size = PyList_Size(keys);

    for (Py_ssize_t i = 0; i < size; i++) {
        // Ключ
        PyObject *key = PyList_GetItem(keys, i);
        if (!PyUnicode_Check(key)) {
            Py_DECREF(keys);
            return -1;
        }

        PyObject *key_bytes = PyUnicode_AsEncodedString(key, "utf-8", "~E~");
        if (!key_bytes) {
            Py_DECREF(keys);
            return -1;
        }
        const char *key_str = PyBytes_AsString(key_bytes);

        fprintf(out, "\"%s\":", key_str);
        Py_DECREF(key_bytes);

        // Значение
        PyObject *val = PyDict_GetItem(dict, key);  // borrowed reference
        if (!val) {
            Py_DECREF(keys);
            return -1;
        }
        if (cjson_dump_value(val, out) < 0) {
            Py_DECREF(keys);
            return -1;
        }

        if (i < size - 1) {
            fputc(',', out);
        }
    }
    Py_DECREF(keys);

    fputc('}', out);
    return 0;
}

static int cjson_dump_value(PyObject *value, FILE *out) {
    if (PyLong_Check(value)) {

        long v = PyLong_AsLong(value);
        fprintf(out, "%ld", v);
    } else if (PyUnicode_Check(value)) {

        PyObject *val_bytes = PyUnicode_AsEncodedString(value, "utf-8", "~E~");
        if (!val_bytes) {
            return -1;
        }
        const char *val_str = PyBytes_AsString(val_bytes);
        fprintf(out, "\"%s\"", val_str);
        Py_DECREF(val_bytes);
    } else {
        PyErr_SetString(PyExc_TypeError, "Unsupported type for serialization");
        return -1;
    }
    return 0;
}

static PyObject *cjson_dumps(PyObject *self, PyObject *args) {
    PyObject *dict = NULL;

    if (!PyArg_ParseTuple(args, "O!", &PyDict_Type, &dict)) {
        PyErr_SetString(PyExc_TypeError, "Argument must be a dict");
        return NULL;
    }

    FILE *memfile = tmpfile();
    if (!memfile) {
        PyErr_SetString(PyExc_OSError, "Failed to create temporary file");
        return NULL;
    }

    if (cjson_dump_dict(dict, memfile) < 0) {
        fclose(memfile);
        return NULL;
    }

    // Вычисляем размер
    long pos = ftell(memfile);
    if (pos < 0) {
        fclose(memfile);
        PyErr_SetString(PyExc_OSError, "ftell failed");
        return NULL;
    }
    rewind(memfile);

    // Считываем всё в буфер
    char *buffer = (char *)malloc(pos + 1);
    if (!buffer) {
        fclose(memfile);
        PyErr_NoMemory();
        return NULL;
    }

    size_t read_size = fread(buffer, 1, pos, memfile);
    fclose(memfile);
    if ((long)read_size != pos) {
        free(buffer);
        PyErr_SetString(PyExc_OSError, "Failed to read temp file data");
        return NULL;
    }
    buffer[pos] = '\0';

    // Превращаем в Python-строку
    PyObject *res = Py_BuildValue("s", buffer);
    free(buffer);

    return res;
}

// Таблица методов
static PyMethodDef cjson_methods[] = {
    {"loads", (PyCFunction)cjson_loads, METH_VARARGS, "Parse JSON string into a dict"},
    {"dumps", (PyCFunction)cjson_dumps, METH_VARARGS, "Dump dict into a JSON string"},
    {NULL, NULL, 0, NULL}
};

// Описание модуля
static struct PyModuleDef cjson_module = {
    PyModuleDef_HEAD_INIT,
    "cjson",        // имя модуля в Python
    "Example C JSON parser and dumper",  // документация
    -1,             // размер памяти под состояние модуля (-1 если не используется)
    cjson_methods
};

// Функция инициализации модуля
PyMODINIT_FUNC PyInit_cjson(void) {
    return PyModule_Create(&cjson_module);
}
