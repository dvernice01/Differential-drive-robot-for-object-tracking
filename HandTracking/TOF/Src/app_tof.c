#include <platform.h>
#include "app_tof.h"
#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include "vl53l5cx.h"
#include "stm32f4xx_hal.h"
#include "vl53l5cx_api.h"


// Struttura di configurazione del sensore
VL53L5CX_Configuration p_dev;
VL53L5CX_ResultsData results;
static VL53L5CX_DetectionThresholds p_thresholds;

// Dichiarazione esterna dell’I2C di CubeMX
extern I2C_HandleTypeDef hi2c1;

uint8_t frequency = 15;
uint8_t sharpener_percent = 50;
uint8_t i = 0;
uint8_t ready = 0;


float linear_gain = 5.0f;
float angular_gain = 5.0f;
float desired_distance = 300.0f;
float measured_distance;
float center = 4.5f;
float l = 5.0f;
float r = 3.25f;
float error_linear_vel;
volatile float error_angular_vel;
float dc1;
float dc2;
float w;


/**
 * @brief Reset hardware del sensore VL53L5CX.
 */

/*void do_xtalk_calibration_once(void)
{
    uint8_t xtalk_buf[VL53L5CX_XTALK_BUFFER_SIZE];

    // 1. esegui calibrazione (con target alluminio a 600 mm, riflettanza ~90%)
    if (vl53l5cx_calibrate_xtalk(&p_dev, 9, 4, 600) != VL53L5CX_STATUS_OK) {
        printf("Errore calibrazione\n");
        return;
    }

    // 2. recupera i dati di calibrazione
    if (vl53l5cx_get_caldata_xtalk(&p_dev, xtalk_buf) != VL53L5CX_STATUS_OK) {
        printf("Errore get_caldata\n");
        return;
    }

    // 3. stampa o salva i dati (qui stampo in hex per salvarli a mano)
    printf("Dati calibrazione XtALK (da salvare):\n");
    for (int i = 0; i < VL53L5CX_XTALK_BUFFER_SIZE; i++) {
        printf("0x%02X, ", xtalk_buf[i]);
        if ((i+1) % 16 == 0) printf("\n");
    }
}*/

void HardwareReset(void) {

  HAL_GPIO_WritePin(PWR_EN_C_GPIO_Port, PWR_EN_C_Pin, GPIO_PIN_RESET);
  HAL_Delay(2);
  HAL_GPIO_WritePin(PWR_EN_C_GPIO_Port, PWR_EN_C_Pin, GPIO_PIN_SET);
  HAL_Delay(2);
  HAL_GPIO_WritePin(LPn_C_GPIO_Port, LPn_C_Pin, GPIO_PIN_RESET);
  HAL_Delay(2);
  HAL_GPIO_WritePin(LPn_C_GPIO_Port, LPn_C_Pin, GPIO_PIN_SET);
  HAL_Delay(2);
}

/*void I2C_Scan(void) {
    printf("Inizio scansione I2C...\n");

    HAL_StatusTypeDef result;
    int devices_found = 0;

    for (i = 1; i < 128; i++) {
        // Gli indirizzi I2C sono a 7 bit, quindi vanno shiftati di 1
        result = HAL_I2C_IsDeviceReady(&hi2c1, (uint16_t)(i << 1), 2, 10);

        if (result == HAL_OK) {
            printf("Dispositivo trovato a 0x%02X\n", i << 1);
            devices_found++;
        }
    }

    if (devices_found == 0) {
        printf("Nessun dispositivo trovato!\n");
    } else {
        printf("Scansione completata: %d dispositivi trovati.\n", devices_found);
    }
}*/

/**
 * @brief Inizializza il sensore VL53L5CX.
 */
void Tof_init(void) {
    // Configurazione della struttura platform
    p_dev.platform.address = 0x29 << 1;   // 0x29 dal datasheet → shiftato di 1 per HAL
    p_dev.platform.Write   = VL53L5CX_I2C_Write;
    p_dev.platform.Read    = VL53L5CX_I2C_Read;
    p_dev.platform.GetTick = VL53L5CX_GetTick;

    p_thresholds.param_low_thresh = desired_distance + 25;
    p_thresholds.param_high_thresh = desired_distance - 25;
    p_thresholds.measurement = VL53L5CX_DISTANCE_MM;
    p_thresholds.type = VL53L5CX_OUT_OF_WINDOW;
    p_thresholds.zone_num = VL53L5CX_LAST_THRESHOLD;

    // Reset hardware del sensore
    HardwareReset();

    // Tentativo di inizializzazione
    printf("Avvio init VL53L5CX...\n");
    if (vl53l5cx_init(&p_dev) != VL53L5CX_OK) {
        printf("Errore Init\n");
        Error_Handler();
    } else {
        printf("Init OK\n");
    }
}

void Tof_conf(void){


	//do_xtalk_calibration_once();
	/*if (vl53l5cx_set_caldata_xtalk(&p_dev, (uint8_t*)my_saved_xtalk) != VL53L5CX_STATUS_OK) {
		printf("Errore set_caldata\n");
	} else {
		printf("Dati XtALK caricati correttamente\n");
	}*/

    if (vl53l5cx_set_resolution(&p_dev, VL53L5CX_RESOLUTION_8X8) != VL53L5CX_OK) {
        printf("Errore Set Resolution\n");
        Error_Handler();
    } else {
        printf("Set Resolution OK\n");
    }

    if (vl53l5cx_set_target_order(&p_dev, VL53L5CX_TARGET_ORDER_CLOSEST) != VL53L5CX_OK) {
		printf("Errore Set Resolution\n");
		Error_Handler();
	} else {
		printf("Set Resolution OK\n");
	}


	if (vl53l5cx_set_ranging_frequency_hz(&p_dev, frequency) != VL53L5CX_OK) {
		  printf("Errore Set Ranging Frequency\n");
		  Error_Handler();
	  } else {
		  printf("Set Ranging Frequency OK\n");
	}


	if  (vl53l5cx_set_ranging_mode(&p_dev, VL53L5CX_RANGING_MODE_CONTINUOUS) != VL53L5CX_OK) {
		  printf("Errore Set Ranging Mode\n");
		  Error_Handler();
	  } else {
		  printf("Set Ranging Mode OK\n");
	}


	if  (vl53l5cx_set_sharpener_percent(&p_dev, sharpener_percent) != VL53L5CX_OK) {
		  printf("Errore Set sharpener\n");
		  Error_Handler();
	  } else {
		  printf("Set sharpener OK\n");
	}


	/*if  (vl53l5cx_set_detection_thresholds_enable (&p_dev, enable) != VL53L5CX_OK) {
		  printf("Errore Set detection thresholds enable \n");
		  Error_Handler();
	  } else {
		  printf("Set detection thresholds enable OK\n");
	}*/

	if  (vl53l5cx_set_detection_thresholds_enable (&p_dev, 0) != VL53L5CX_OK) {
			  printf("Errore Set detection thresholds enable \n");
			  Error_Handler();
		  } else {
			  printf("Set detection thresholds enable OK\n");
		}

	/*for (i = 0; i < 64; i++) {
		p_thresholds.zone_num = i;
		if  (vl53l5cx_set_detection_thresholds(&p_dev, &p_thresholds) != VL53L5CX_OK) {
			  printf("Errore Set detection thresholds \n");
			  Error_Handler();
		  } else {
			  printf("Set detection thresholds OK\n");
		}
	}*/

	if  (vl53l5cx_start_ranging(&p_dev) != VL53L5CX_OK) {
		  printf("Errore start \n");
		  Error_Handler();
	  } else {
		  printf("start OK\n");
	}

	ready = 1;

}


void feedbackcontroll(void){

	float v_max = fabsf ((desired_distance - (float) 4000.0));
	float w_max = fabsf (((float)8.0 - center));
	float phi_max = (v_max + l * w_max) / r;


	int num_col_1 = 0;
	float sum_distance_col_1 = 0;
	//float distance_col_1 = 0;

	int num_col_2 = 0;
	float sum_distance_col_2 = 0;
	//float distance_col_2 = 0;

	int num_col_3 = 0;
	float sum_distance_col_3 = 0;
	//float distance_col_3 = 0;

	int num_col_4 = 0;
	float sum_distance_col_4 = 0;
	//float distance_col_4 = 0;

	int num_col_5 = 0;
	float sum_distance_col_5 = 0;
	//float distance_col_5 = 0;

	int num_col_6 = 0;
	float sum_distance_col_6 = 0;
	//float distance_col_6 = 0;

	int num_col_7 = 0;
	float sum_distance_col_7 = 0;
	//float distance_col_7 = 0;

	int num_col_8 = 0;
	float sum_distance_col_8 = 0;
	//float distance_col_8 = 0;

	for (i = 0; i < 64; i++){

		if (results.target_status[i] == 5){

			if ( i==0 || i==8 || i==16 || i==24 ||i==32 || i==40 || i==48 || i==56 ) {

				 num_col_1 = num_col_1 + 1; //conto quante volte l'indice appartiene a questa colonna
				 sum_distance_col_1 = sum_distance_col_1 + results.distance_mm[i]; //calcolo ma somma delle distanze appartenenti a quella colonna
			}

			if ( i==1 || i==9 || i==17 || i==25 ||i==33 || i==41 || i==49 || i==57 ) {

				 num_col_2 = num_col_2 + 1;
				 sum_distance_col_2 = sum_distance_col_2 + results.distance_mm[i];
			}

			if (i==2 || i==10 || i==18 || i==26 ||i==34 || i==42 || i==50 || i==58 ) {

				 num_col_3 = num_col_3 + 1;
				 sum_distance_col_3 = sum_distance_col_3 + results.distance_mm[i];

			}

			if ( i==3 || i==11 || i==19 || i==27 ||i==35 || i==43 || i==51 || i==59 ) {

				 num_col_4 = num_col_4 + 1;
				 sum_distance_col_4 = sum_distance_col_4 + results.distance_mm[i];

			}

			if ( i==4 || i==12 || i==20 || i==28 ||i==36 || i==44 || i==52 || i==60 ) {

				 num_col_5 = num_col_5 + 1;
				 sum_distance_col_5 = sum_distance_col_5 + results.distance_mm[i];

			}

			if ( i==5 || i==13 || i==21 || i==29 ||i==37 || i==45 || i==53 || i==61 ) {

				 num_col_6 = num_col_6 + 1;
				 sum_distance_col_6 = sum_distance_col_6 + results.distance_mm[i];

			}

			if ( i==6 || i==14 || i==22 || i==30 ||i==38 || i==46 || i==54 || i==62 ) {

				 num_col_7 = num_col_7 + 1;
				 sum_distance_col_7 = sum_distance_col_7 + results.distance_mm[i];

			}

			if ( i==7 || i==15 || i==23 || i==31 ||i==39 || i==47 || i==55 || i==63 ) {

				 num_col_8 = num_col_8 + 1;
				 sum_distance_col_8 = sum_distance_col_8 + results.distance_mm[i];
			}

		}
	}

		int total_cols = num_col_1 + num_col_2 + num_col_3 + num_col_4 + num_col_5 + num_col_6 + num_col_7 + num_col_8 ;

		if (total_cols <= 5){ //ritenuto il numero di celle minimo per determinare la presenza dell'obiettivo
			return;
		}


		int num_cols[8] = {
		    num_col_1, num_col_2, num_col_3, num_col_4,
		    num_col_5, num_col_6, num_col_7, num_col_8
		};


		int max_index = 0;   // indice della colonna con più punti
		int max_value = num_cols[0];

		for (int k = 1; k < 8; k++) {
		    if (num_cols[k] >= max_value) {
		        max_value = num_cols[k];
		        max_index = k;
		    }
		}

		if (max_index == 0){

			measured_distance = sum_distance_col_1 / num_col_1;

		} else if (max_index == 1){

			measured_distance = sum_distance_col_2 / num_col_2;

		} else if (max_index == 2){

			measured_distance = sum_distance_col_3 / num_col_3;

		} else if (max_index == 3){

			measured_distance = sum_distance_col_4 / num_col_4;

		} else if (max_index == 4){

			measured_distance = sum_distance_col_5 / num_col_5;

		} else if (max_index == 5){

			measured_distance = sum_distance_col_6 / num_col_6;

		} else if (max_index == 6){

			measured_distance = sum_distance_col_7 / num_col_7;

		} else if (max_index == 7){

			measured_distance = sum_distance_col_8 / num_col_8;

		}

		printf("measured distance %d\n", (int)measured_distance);


	    // Controllo velocità lineare (in avanti o indietro)
		error_linear_vel = (measured_distance - desired_distance) ;

	    float v = linear_gain * error_linear_vel;

	    // l'if è messo perchè il centro è 4,5 visto che le colonne sono 8 quindi
	    // potevo oscillare tra 3 e 4 e avere un comportamento anomalo
	    max_index ++;
	    if (max_index == 4 || max_index == 5) { //perchè max index parte da 0

	    	error_angular_vel = 0.0;
	    	w = 0.0;
	    } else{

		    error_angular_vel = (max_index - center);
	    	w = (angular_gain * error_angular_vel);
	    }


	    // Calcola le velocità delle ruote
	    float phi1 = (v + l * w) / r;
	    float phi2 = (v - l * w) / r;

	    dc1 = (phi1/phi_max);
	    dc2 = (phi2/phi_max);


	    printf("dc1 %d\n", (int) (dc1*100));
	    printf("dc2 %d\n", (int) (dc2*100));


	    if (dc1 > 1.0f) dc1 = 1.0f;
	    if (dc1 < -1.0f) dc1 = -1.0f;

	    if (dc2 > 1.0f) dc2 = 1.0f;
	    if (dc2 < -1.0f) dc2 = -1.0f;

	    if (dc1 > 0.02f && dc1 < 0.2f) dc1 = 0.2f;
	    if (dc2 > 0.02f && dc2 < 0.2f) dc2 = 0.2f;

	    if (dc1 < -0.02f && dc1 > -0.2f) dc1 = -0.2f;
	    if (dc2 < -0.02f && dc2 > -0.2f) dc2 = -0.2f;

	    if ((dc1 > -0.02f && dc1 < 0.02f) && (dc1 > -0.02f && dc1 < 0.02f)) dc1 = 0.0f;
	    if ((dc2 > -0.02f && dc2 < 0.02f) && (dc2 > -0.02f && dc2 < 0.02f)) dc2 = 0.0f;


	    uint32_t ccr1 = (uint32_t) (fabsf(dc1) * (float) (1 + htim2.Init.Period));
	    uint32_t ccr2 = (uint32_t) (fabsf(dc2) * (float) (1 + htim3.Init.Period));


	    if (dc1 > 0.0f) {
	        __HAL_TIM_SET_COMPARE (&htim2, TIM_CHANNEL_1, ccr1);
	        __HAL_TIM_SET_COMPARE (&htim2, TIM_CHANNEL_2, 0);
	    }
	    else if (dc1==0.0f) {
	        __HAL_TIM_SET_COMPARE (&htim2, TIM_CHANNEL_1, 0);
	        __HAL_TIM_SET_COMPARE (&htim2, TIM_CHANNEL_2, 0);
	    }
	    else {
	        __HAL_TIM_SET_COMPARE (&htim2, TIM_CHANNEL_1, 0);
	        __HAL_TIM_SET_COMPARE (&htim2, TIM_CHANNEL_2, ccr1);
	    }

	    if (dc2 > 0.0f) {
	        __HAL_TIM_SET_COMPARE (&htim3, TIM_CHANNEL_1, ccr2);
	        __HAL_TIM_SET_COMPARE (&htim3, TIM_CHANNEL_2, 0);
	    }
	    else if (dc2 == 0.0f) {
	        __HAL_TIM_SET_COMPARE (&htim3, TIM_CHANNEL_1, 0);
	        __HAL_TIM_SET_COMPARE (&htim3, TIM_CHANNEL_2, 0);
	    }
	    else {
	        __HAL_TIM_SET_COMPARE (&htim3, TIM_CHANNEL_1, 0);
	        __HAL_TIM_SET_COMPARE (&htim3, TIM_CHANNEL_2, ccr2);
	    }

}

/*printf("total_cols: %d\n", total_cols);
float index = (total_cols > 0) ? ((num_col_1*1 + num_col_2*2 + num_col_3*3 + num_col_4*4 + num_col_5*5 + num_col_6*6 + num_col_7*7 + num_col_8*8) / (float)total_cols) : 0;


//float index = ((col_1*1) + (col_2*2) + (col_3*3) + (col_4*4) + (col_5*5) + (col_6*6) + (col_7*7) + (col_8*8))/(col_1 + col_2 + col_3 + col_4 + col_5 + col_6 + col_7 + col_8);
//printf("index float %f\n", index); // @suppress("Float formatting support")
if ((int)index == 1){
	measured_distance = distance_col_1;
	if (measured_distance == 0){
		measured_distance = distance_col_2;
	}
} else if ((int)index == 2) {
	measured_distance = distance_col_2;
	if (measured_distance == 0){
					measured_distance = distance_col_3;
				}
} else if ((int)index == 3) {
	measured_distance = distance_col_3;
	if (measured_distance == 0){
					measured_distance = distance_col_4;
				}
} else if ((int)index == 4) {
	measured_distance = distance_col_4;
	if (measured_distance == 0){
					measured_distance = distance_col_5;
				}
} else if ((int)index == 5) {
	measured_distance = distance_col_5;
	if (measured_distance == 0){
					measured_distance = distance_col_6;
				}
} else if ((int)index == 6) {
	measured_distance = distance_col_6;
	if (measured_distance == 0){
					measured_distance = distance_col_7;
				}
} else if ((int)index == 7) {
	measured_distance = distance_col_7;
	if (measured_distance == 0){
					measured_distance = distance_col_8;
				}
} else if ((int)index == 8) {
	measured_distance = distance_col_8;
}*/
