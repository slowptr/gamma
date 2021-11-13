#include <math.h>
#include <stdio.h>
#include <windows.h>

typedef int(__stdcall *type)(void *hdc, void *gamma_ramp);

int main(int argc, char *argv[]) {
    int gamma            = 128;
    int gamma_percentage = 50;  // i should prob. calculate here
    if (argc == 2) {
        gamma_percentage = atoi(argv[1]);
        if (gamma_percentage > 100 || gamma_percentage < 15) {
            printf("[-] please enter a value between 15 and 100.\n");
            return 1;
        }

        float tmp = ((float)gamma_percentage / 100) * 256;  // percentage calculation
        gamma     = (int)tmp;
    }
    HMODULE h_gdi32 = LoadLibrary("gdi32.dll");
    if (!h_gdi32) {
        printf("[-] couldn't get gdi32.dll\n");
        return 1;
    }

    type get_gamma = GetProcAddress(h_gdi32, "GetDeviceGammaRamp");
    type set_gamma = GetProcAddress(h_gdi32, "SetDeviceGammaRamp");

    HDC cur_hdc = GetDC(NULL);

    unsigned short gamma_table[3][256];
    get_gamma(cur_hdc, &gamma_table);

    // calculate percentage of gamma_table (random point in it, rn)
    int   check = 181;  // more random than thought of tbh
    float tmp   = (((float)gamma_table[0][check] / check - 128) / 256) * 100;
    // v = (t[c][i] / i) - 128; (c = color)

    printf("[+] before screen brightness: %i%%\n", (int)ceilf(tmp));  // always round up

    for (int i = 0; i < 256; i++) {
        int array = i * (gamma + 128);
        if (array > 65535) array = 65535;

        gamma_table[0][i] = gamma_table[1][i] = gamma_table[2][i] = (unsigned short)array;
    }

    if (!set_gamma(cur_hdc, gamma_table))
        printf("[-] SetDeviceGammaRamp failed.\n");
    else
        printf("[+] after screen brightness:  %i%%\n", gamma_percentage);  // space for alignment

    ReleaseDC(NULL, cur_hdc);
    return 0;
}