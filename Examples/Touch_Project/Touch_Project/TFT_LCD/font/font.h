// Font data for Lucida Console 12pt

// Структура доступа к массиву индексов символов
typedef struct FontTable
{
 uint16_t width;           // Ширина символа
 uint16_t start;           // Стартовый индекс на первый байт символа в массиве данных символов
} FONT_CHAR_INFO;

// Структура для доступа к параметрам используемого шрифта
typedef struct
{
 uint8_t Height;                        // Высота символов
 uint8_t FirstChar;                     // Индекс первого символа
 uint8_t LastChar;                      // Индекс последнего символа
 uint8_t FontSpace;                     // Пробел между символами
 const FONT_CHAR_INFO *FontTable;    // Таблица индексов символов
 const uint8_t *FontBitmaps;            // Указатель на массив с данными о символах
} FONT_INFO;
