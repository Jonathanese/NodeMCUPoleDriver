float gamma   = 2.2; // Correction factor
int   max_in  = 255, // Top end of INPUT range
      max_out = 65535, // Top end of OUTPUT range
      r = 255,
      g = 224,
      b = 140;

PrintWriter file;

void setup() {
  file = createWriter("../Color_Correction.h");
  float scalar = 0;
  if(r>scalar) scalar = r;
  if(g>scalar) scalar = g;
  if(b>scalar) scalar = b;
  file.print("#define CORRECTION_PRECISION ");
  file.println((int)(max_out/max_in));
  addArray("PRECISE_RED", max_out*r/scalar);
  addArray("PRECISE_GREEN", max_out*g/scalar);
  addArray("PRECISE_BLUE", max_out*b/scalar);
  
  file.flush();
  file.close();
  exit();
}

void addArray(String name, float max)
{
    file.print("const uint16_t PROGMEM ");
    file.print(name);
    file.print("[] = {");
  for(int i=0; i<=max_in; i++) {
    if(i > 0) file.print(',');
    if((i & 15) == 0) file.print("\n  ");
    file.print((int)(pow((float)i / (float)max_in, gamma) * max + 0.5));
  }
  file.println(" };");
  file.println();
}
