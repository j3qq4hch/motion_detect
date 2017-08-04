int mSLEEP_15MS = 15;
int mSLEEP_30MS = 30;
int mSLEEP_60MS = 60;
int mSLEEP_120MS = 120;
int mSLEEP_250MS = 250;
int mSLEEP_500MS = 500;
int mSLEEP_1S = 1000;
int mSLEEP_2S = 2000;
int mSLEEP_4S = 4000;
int mSLEEP_8S = 8000;
//    int SLEEP_FOREVER


void sleep_delay(int sleep_time) {
  delay(sleep_time);
  //  LowPower.idle(sleep_time);
}
