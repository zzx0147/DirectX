#pragma once
//x가 true라면 Y를 리턴
#define isValidReturn(x,y) if(x) {return y;}
//x가 false라면 Y를 리턴
#define isNotValidReturn(x,y) isValidReturn(!x,y)
//x가 true라면 x의 Release()를 호출한 뒤 x = 0
#define isValidRelease(x) if(x) {x->Release() ;x = nullptr;}
//x가 true라면 x의 Shutdown()을 호출한 뒤 delete x, 후 x = 0;
#define isValidShutdown(x) if(x) {x->Shutdown() ; delete x; x = nullptr;}
//x가 true라면 delete x 후 x = 0
#define isValidDelete(x) if(x) {delete x; x = nullptr;}

