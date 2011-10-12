/*
Esta clase tiene como finalidad identificar desde el InputManager un canal en un dispositivo.
Por ejemplo, si muiDevice vale 0 y muiChannel es 18, se interpretaría como el 
canal 18 en el dispositivo 0 del InputManager.
*/

#ifndef INPUT_ENTRY_H
#define INPUT_ENTRY_H
 
class cInputEntry
{     
    public:
        unsigned muiDevice;
        unsigned muiChannel;
};
 
#endif