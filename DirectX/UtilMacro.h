#pragma once
//x�� true��� Y�� ����
#define isValidReturn(x,y) if(x) {return y;}
//x�� false��� Y�� ����
#define isNotValidReturn(x,y) isValidReturn(!x,y)
//x�� true��� x�� Release()�� ȣ���� �� x = 0
#define isValidRelease(x) if(x) {x->Release() ;x = nullptr;}
//x�� true��� x�� Shutdown()�� ȣ���� �� delete x, �� x = 0;
#define isValidShutdown(x) if(x) {x->Shutdown() ; delete x; x = nullptr;}
//x�� true��� delete x �� x = 0
#define isValidDelete(x) if(x) {delete x; x = nullptr;}

