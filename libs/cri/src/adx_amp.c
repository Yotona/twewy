// ADXAMP (amplitude extraction) is compiled out on NITRO. Every entry point
// the ADXT core still calls is an empty body.

void ADXAMP_Start(void* amp) {
    return; // Do nothing
}

void ADXAMP_Stop(void* amp) {
    return; // Do nothing
}

void ADXAMP_SetSfreq(void* amp, int sfreq) {
    return; // Do nothing
}

void ADXAMP_Destroy(void* amp) {
    return; // Do nothing
}
