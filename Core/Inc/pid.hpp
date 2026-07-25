#ifndef PID_HPP
#define PID_HPP

class PID {
public:
    float Kp, Ki, Kd;
    float error_sum;
    float last_error;
    float output_limit;
    PID();
    PID(float p, float i, float d, float limit); // 預設建構子
    void setParameters(float p, float i, float d, float limit); // 設定參數
    float compute(float target, float current, float dt);       // 核心運算
    void reset();                                               // 積分歸零
};

#endif // PID_HPP