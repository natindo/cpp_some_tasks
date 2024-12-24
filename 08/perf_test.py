import timeit
import json
import ujson
import cjson

# Генерируем ОДИН огромный JSON-объект:
keys_values = []
for i in range(100000):
    keys_values.append(f"\"key{i}\": \"Some string value {i}\"")

BIG_JSON = "{" + ",".join(keys_values) + "}"

def bench(module, data):
    setup = f"import {module.__name__}; j = '''{data}'''"
    stmt_loads = f"{module.__name__}.loads(j)"
    time_loads = timeit.timeit(stmt_loads, setup=setup, number=100)
    print(f"{module.__name__}.loads: {time_loads:.4f} seconds for 100 runs")

    obj = module.loads(data)

    setup_dumps = f"import {module.__name__}; import json; obj={obj!r}"
    stmt_dumps = f"{module.__name__}.dumps(obj)"
    time_dumps = timeit.timeit(stmt_dumps, setup=setup_dumps, number=100)
    print(f"{module.__name__}.dumps: {time_dumps:.4f} seconds for 100 runs")
    print()

def main():
    print("Testing performance on one *big* JSON dict...\n")
    bench(json, BIG_JSON)
    bench(ujson, BIG_JSON)
    bench(cjson, BIG_JSON)

if __name__ == "__main__":
    main()
