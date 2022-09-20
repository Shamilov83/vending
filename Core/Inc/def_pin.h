/*
 * def_pin.h
 *
 *  Created on: 20 сент. 2022 г.
 *      Author: shamilov
 */

#ifndef INC_DEF_PIN_H_
#define INC_DEF_PIN_H_


/*кнопки пульта*/
//ШД
#define stp_fr 0
#define stp_bk 1
//Штамп
#define sht_opn 2
#define sht_cls 3
//Магн
#define mg_fr 4
#define mg_bk 5
//Нож
#define cut_opn 6
#define cut_cls 7

#define Res_USB GPIOB,GPIO_PIN_7
//шаговый двигатель
#define EN_STEP_MOT GPIOB,GPIO_PIN_12
#define DIR_STEP_MOT GPIOB,GPIO_PIN_14
#define STEP_STEP_MOT GPIOB,GPIO_PIN_13
//двигатель штампа
#define DRAW_KD1_A GPIOA,GPIO_PIN_10
#define DRAW_KD1_B GPIOA,GPIO_PIN_9
#define MOT_SHTAMP  DRAW_KD1_A,DRAW_KD1_B

//двигатель выброса. сторона А, сторона Б
#define DRAW_KD2_A GPIOA,GPIO_PIN_8
#define DRAW_KD2_B GPIOB,GPIO_PIN_15
#define MOT_EJECT DRAW_KD2_A,DRAW_KD2_B

//двигатель ножа1
#define DRAW_KD3_A GPIOB,GPIO_PIN_10
#define DRAW_KD3_B GPIOB,GPIO_PIN_11
#define MOT_CUT DRAW_KD3_A,DRAW_KD3_B

//двигатель магнита
#define DRAW_KD4_A GPIOB,GPIO_PIN_1
#define DRAW_KD4_B GPIOB,GPIO_PIN_0
#define MOT_MAGN DRAW_KD4_A,DRAW_KD4_B
//соленойд выравнивателя
#define SOL1_ALIGN GPIOA,GPIO_PIN_4
//соленойд поджима
#define SOL2_PRESS GPIOA,GPIO_PIN_5
//соленойд склеивателя
#define SOL3_GLUE GPIOA,GPIO_PIN_6
//выдача. сторона А, сторона Б
#define SOL4_EJECT GPIOA,GPIO_PIN_7
//выход ИК светодиодов
#define UR_LED GPIOA,GPIO_PIN_3
//вход татчика тока штампа
#define DT_SHTAMP GPIOA,GPIO_PIN_1
//вход энкодера штампа
#define ENCODER GPIOA,GPIO_PIN_2
//шина I2C
#define I2C_CLK GPIOB,GPIO_PIN_8
#define I2C_DA GPIOB,GPIO_PIN_9
//вход
#define Srv_mod GPIOB,GPIO_PIN_5
#define KV4 GPIOB,GPIO_PIN_6
//вход
#define KV2 GPIOC,GPIO_PIN_13
#define KV1 GPIOA,GPIO_PIN_15
//оптодатчики
#define opto_print_in 0
#define opto_print_2 1
#define opto_magn 2
#define kv_cut_up 3
#define kv_sht_open 4
#define kv_cut_down 5
#define opto_6 6
#define opto_7 7



#endif /* INC_DEF_PIN_H_ */
