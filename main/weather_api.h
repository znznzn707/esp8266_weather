#ifndef WEATHER_API_H

typedef struct weather_now_resp_body
{
    char m_text[16];                               //天气现象文字
    unsigned char m_code;                          //天气现象代码
    unsigned char m_temperature;                   //温度或湿度
}weather_now_resp_body_t;              //天气实况响应体



void weather_get_now(char *key, char *location, char *language, char *unit,
                     weather_now_resp_body_t *now_resp_body);

#endif // !WEATHER_API_H
