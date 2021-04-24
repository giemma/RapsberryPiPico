



class Stepper {
    private :

        uint8_t Pin1;
        uint8_t Pin2;
        uint8_t Pin3;
        uint8_t Pin4;
        uint8_t Speed;
        
        bool isRunning;

        void SingleStep(int step) ;
        void MoveSteps(int steps);
    public:

        Stepper(uint8_t pin1,uint8_t pin2, uint8_t pin3, uint8_t pin4, uint8_t speed);

        void MoveAngle(int angle);
        void Stop();
        bool IsRunning();

        uint8_t GetSpeed();
        void SetSpeed( uint8_t speed);
     };