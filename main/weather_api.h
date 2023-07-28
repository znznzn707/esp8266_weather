#ifndef WEATHER_API_H

typedef struct weather_now_resp_body
{
    char m_text[16];                               //天气现象文字
    unsigned char m_code;                          //天气现象代码
    unsigned char m_temperature;                   //温度或湿度
}weather_now_resp_body_t;              //天气实况响应体

//TODO
typedef struct weather_daily_resp_body
{
}weather_daily_resp_body_t;              //未来三天天气响应体


//------------------------------------------------------------------------------------
/**
 * @brief 获取当天天气实况
 * 
 * @param key 私钥
 * @param location 位置 英文/中文 
 * @param language 语言
 * @param unit c 表示温度；f 表示湿度
 * @param now_resp_body 
 */
void weather_get_now(char *key, char *location, char *language, char *unit,
                     weather_now_resp_body_t *now_resp_body);


//TODO
//---------------------------------------------------------------------------------------
/**
 * @brief 获取未来三天的天气
 * 
 * @param key 私钥
 * @param location 位置 英文/中文 
 * @param daily_resp_body 
 */
void weather_get_daily(char *key, char *location, char *language,
                       weather_daily_resp_body_t *daily_resp_body);

#endif // !WEATHER_API_H
