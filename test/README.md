## Test-Suite/src/tests/api-gishelper-sld

Тест проверяет приватное API config_parser_api.h:
1. Создаёт файл $CLASS_FILTER_CONFIG_FNAME
2. Добавляет в него "Test-section"
3. Ищет только что записанную секцию и сравнивает размер списка индексов классов.
