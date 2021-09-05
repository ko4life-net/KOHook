#pragma once

struct AppData
{
    // http://www.c-cam.be/doc/Archive/FrameRates.pdf
    float * fSecPerFrm;
    float * fFrmPerSec;
    float   fSecPerFrmPrecise;
    bool    bFPSControlEnabled;

    AppData()
    {
        fSecPerFrm = reinterpret_cast<float *>(0x006D3AB0);
        fFrmPerSec = reinterpret_cast<float *>(0x006D3AAC);
        fSecPerFrmPrecise = 14000.0f / 1000000.0f; // precise frames per sec = (11111 / 1,000,000 nanoseconds)
        bFPSControlEnabled = true;
    }
};

class App
{
public:
    void Start();
    void Stop();

    void StartGameLoop();

    static App & Instance()
    {
        static App s_Instance;
        return s_Instance;
    }

    App(const App &) = delete;
    App & operator=(const App &) = delete;

    static AppData s_Data;

private:
    App();

};
