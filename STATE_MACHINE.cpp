#include "mainwindow.h"
#include <QApplication>
#include <QDebug>



typedef enum{STATE_A,STATE_B,STATE_C,STATE_D,NUM_STATES}StateType;
StateType fsmState = STATE_A;
typedef enum{RETURN_OK,RETURN_FAIL,RETURN_REPEAT}ReturnCodes;

ReturnCodes entry_state(void){qDebug()<<"entry";return RETURN_OK;}
ReturnCodes foo_state(void){qDebug()<<"foo";return RETURN_OK;}
ReturnCodes bar_state(void){qDebug()<<"bar";return RETURN_OK;}
ReturnCodes baz_state(void){qDebug()<<"baz";return RETURN_OK;}


typedef struct{
    StateType CurrentState;
    ReturnCodes ReturnState;
    StateType NextState;
    ReturnCodes(*fptr)(void);
}StateMachineType;

static const StateMachineType FSM[] =
{
//  Current   Transition  Next
//  State ID   Letter   State ID
    {STATE_A ,RETURN_OK ,STATE_B ,entry_state},
    {STATE_B ,RETURN_OK ,STATE_C ,foo_state},
    {STATE_C ,RETURN_OK ,STATE_D ,bar_state},
    {STATE_D ,RETURN_OK ,STATE_A ,baz_state},
};

StateMachineType const *table_begin(void);
StateMachineType const *table_end(void);
void lookup_transition(StateType fsmstate, ReturnCodes returnCodes);
static const int SIZE_FSM = sizeof(FSM)/sizeof(FSM[0]);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    ReturnCodes rc;
    while(1){
        rc = (*FSM[fsmState].fptr)();
        lookup_transition(fsmState,rc);
    }
    return a.exec();
}

StateMachineType const *
table_begin(void)
{
    return &FSM[0];
}

StateMachineType const *
table_end(void)
{
    return &FSM[SIZE_FSM];
}
//! Buscamos el estado de transicion
void lookup_transition(StateType fsmstate, ReturnCodes returnCodes)
{
    StateMachineType const * pEntry = table_begin();
    StateMachineType const * pEnd = table_end();
    bool bStateFound = false;
    while ((!bStateFound) && (pEntry != pEnd)){
        if (pEntry->CurrentState == fsmstate){
            if (pEntry->ReturnState == returnCodes){
                fsmState = pEntry->NextState;
                bStateFound = true;
                break;
            }
        }
        ++pEntry;
    }
}
