#! /usr/bin/env python3

import json
import ujson
import cjson

def test_correctness():
    json_str = '{"hello": 10, "world": "value"}'

    # Парсинг
    py_json_doc = json.loads(json_str)
    ujson_doc = ujson.loads(json_str)
    cjson_doc = cjson.loads(json_str)

    # Проверяем, что все три результата идентичны
    assert py_json_doc == ujson_doc == cjson_doc

    # Проверяем обратную сериализацию
    # Сериализуем dict с помощью cjson, должен получиться эквивалентный json_str
    dumped_str = cjson.dumps(cjson_doc)

    # Можно проверить через парсинг, т.к. json-строки могут отличаться пробелами
    # (хотя в нашем случае не должны)
    assert json.loads(json_str) == json.loads(dumped_str)

def main():
    test_correctness()
    print("All correctness tests passed!")

if __name__ == "__main__":
    main()
