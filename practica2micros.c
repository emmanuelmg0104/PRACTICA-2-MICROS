#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
/* TODO: insert other include files here. */

/* TODO: insert other definitions and declarations here. */

/*
 * @brief   Application entry point.
 */

	typedef struct{
		unsigned char bCurrentState;
		unsigned char bPreviousState;
	}tStateMachine;

	typedef struct{
		unsigned char bFirstData;
		unsigned char bSecondData;
		unsigned char bOperator;
		short wResult;
		short waDataBuffer[5];
		unsigned char bOperationCounter;
	}tCalcData;


	//prototipos void fnYourOwnName (tStateMachine * pSMStruct, tCalcData * pOperationData);

	void fnInitialState(tStateMachine *MaqEstado, tCalcData *DatosCalc);
	void fnWaitFirstDataState(tStateMachine *MaqEstado, tCalcData *DatosCalc);
	void fnWaitOperatorState(tStateMachine *MaqEstado, tCalcData *DatosCalc);
	void fnWaitSecondDataState(tStateMachine *MaqEstado, tCalcData *DatosCalc);
	void fnExecuteOperationState(tStateMachine *MaqEstado, tCalcData *DatosCalc);
	void fnSaveResultState(tStateMachine *MaqEstado, tCalcData *DatosCalc);
	void fnCalculateAverageState(tStateMachine *MaqEstado, tCalcData *DatosCalc);
	void fnResetState(tStateMachine *MaqEstado, tCalcData *DatosCalc);

	unsigned char Transicion;


int main(void) {
	tStateMachine MaqEstado;
			tCalcData DatosCalc;

	void (*pArrayFn[])(tStateMachine*, tCalcData*) = {fnInitialState,  fnWaitFirstDataState, fnWaitOperatorState,
			fnWaitSecondDataState,  fnExecuteOperationState,  fnSaveResultState,  fnCalculateAverageState,  fnResetState};
	Transicion = 0;

	while(1){

		(*pArrayFn[Transicion])(&MaqEstado, &DatosCalc);


	}

}

//----------------------//
void fnInitialState(tStateMachine *MaqEstado, tCalcData *DatosCalc) {
	MaqEstado->bCurrentState = 0;
	MaqEstado->bPreviousState = 0;
	DatosCalc->bFirstData = 0;
	DatosCalc->bSecondData = 0;
	DatosCalc->bOperator = 0;
	DatosCalc->wResult = 0;
	DatosCalc->waDataBuffer[0] = 0;
	DatosCalc->waDataBuffer[1] = 0;
	DatosCalc->waDataBuffer[2] = 0;
	DatosCalc->waDataBuffer[3] = 0;
	DatosCalc->waDataBuffer[4] = 0;
	DatosCalc->bOperationCounter = 0;
	Transicion = 1;
}
//---------------------//
void fnWaitFirstDataState(tStateMachine * MaqEstado, tCalcData * DatosCalc){
	MaqEstado->bCurrentState = Transicion;

	do {
	DatosCalc->bFirstData;   //se cicla
	}while (DatosCalc->bFirstData >= 0xFF);

	MaqEstado->bPreviousState = Transicion;

	Transicion = 2;

}
//---------------------//
void fnWaitOperatorState(tStateMachine * MaqEstado, tCalcData * DatosCalc) {
	MaqEstado->bCurrentState = Transicion;
	DatosCalc->bOperator;
	MaqEstado->bPreviousState = Transicion;
	 if(DatosCalc->bOperator == '+'||DatosCalc->bOperator == '-'||DatosCalc->bOperator == '/'||DatosCalc->bOperator == '*'||DatosCalc->bOperator == '%')
	 	 {
		 Transicion = 3;
	 	 }
	 	 else
	 	 	 {
	 		   Transicion = 7;
	 	 	   }
	 }

//---------------------//
void fnWaitSecondDataState(tStateMachine * MaqEstado, tCalcData * DatosCalc) {
	MaqEstado->bCurrentState = Transicion;

	do {
	DatosCalc->bSecondData;
	}while (DatosCalc->bSecondData == 0xFF||DatosCalc->bSecondData > 0xFF);

	MaqEstado->bPreviousState = Transicion;
	Transicion = 4;
}
//---------------------//
void fnExecuteOperationState(tStateMachine * MaqEstado, tCalcData * DatosCalc) {
	MaqEstado->bCurrentState = Transicion;

	if(DatosCalc->bOperator=='+'){
		DatosCalc->wResult=DatosCalc->bFirstData+DatosCalc->bSecondData;
		}
		else if(DatosCalc->bOperator=='-'){
			DatosCalc->wResult=DatosCalc->bFirstData-DatosCalc->bSecondData;
		}
		else if(DatosCalc->bOperator=='/'){
			DatosCalc->wResult=DatosCalc->bFirstData/DatosCalc->bSecondData;
		}
		else if(DatosCalc->bOperator=='*'){
			DatosCalc->wResult=DatosCalc->bFirstData*DatosCalc->bSecondData;
		}
		else if(DatosCalc->bOperator=='%'){
			DatosCalc->wResult=DatosCalc->bFirstData%DatosCalc->bSecondData;
		}
		DatosCalc->bOperationCounter++;

		MaqEstado->bPreviousState = Transicion;
		Transicion = 5;
}
//---------------------//
void fnSaveResultState(tStateMachine * MaqEstado, tCalcData * DatosCalc) {

	MaqEstado->bCurrentState = Transicion;

	DatosCalc->waDataBuffer[DatosCalc->bOperationCounter - 1]=DatosCalc->wResult;
	MaqEstado->bPreviousState = Transicion;
	if(DatosCalc->bOperationCounter == 5){
		Transicion = 6;
	}
		else{
				Transicion = 7;
			}
}

//---------------------//
void fnCalculateAverageState(tStateMachine * MaqEstado, tCalcData * DatosCalc) {
	MaqEstado->bCurrentState = Transicion;
	unsigned char x = 0;
		for(unsigned char i = 0; i<=4 ; i++){
			x = x + DatosCalc->waDataBuffer[i];
		}

		DatosCalc->wResult = x / 5;
		MaqEstado->bPreviousState = Transicion;

		Transicion = 7;
}

//---------------------//


void fnResetState(tStateMachine * MaqEstado, tCalcData * DatosCalc) {
	MaqEstado->bCurrentState = Transicion;

	DatosCalc->bFirstData = 0xFF;
	DatosCalc->bSecondData = 0xFF;
	DatosCalc->bOperator = 0xFF;
	DatosCalc->wResult = 0xFF;

	MaqEstado->bPreviousState = Transicion;
	Transicion = 1;
}
//---------------------//


