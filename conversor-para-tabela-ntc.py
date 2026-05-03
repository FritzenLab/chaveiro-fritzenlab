import math

# ========== CHANGE THESE ==========
B    = 3950      # beta from your thermistor datasheet
R0   = 10000     # nominal resistance at 25°C
T0   = 298.15    # 25°C in Kelvin
Rser = 10000     # series resistor
Vcc  = 3.0       # PIC12F675 runs on 5V
ADC_MAX = 1023   # 10-bit ADC
# ==================================

T_START = -5.0
T_END   = 45.0
STEP    = 1.0
steps   = int((T_END - T_START) / STEP) + 1  # 51 entries

print(f"#define NTC_T_START  {int(T_START)}")
print(f"#define NTC_ENTRIES  {steps}")
print()
print("const int8_t ntc_adc_table[] = {")

vals = []
for i in range(steps):
    T_c = T_START + i * STEP
    T_k = T_c + 273.15
    R   = R0 * math.exp(B * (1.0/T_k - 1.0/T0))
    # Rser on top, NTC to GND
    Vout = Vcc * Rser / (Rser + R)
    adc  = int(round(Vout / Vcc * ADC_MAX))
    adc  = max(0, min(1023, adc))
    # Scale 10-bit (0-1023) down to 8-bit (0-255) for compact storage
    adc8 = adc >> 2  # divide by 4
    vals.append(adc8)

print(", ".join(str(v) for v in vals))
print("};")