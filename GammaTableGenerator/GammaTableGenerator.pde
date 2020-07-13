float gamma   = 2.2; // Correction factor
long  max_in  = 255, // Top end of INPUT range
      max_out = 65535, // Top end of OUTPUT range
      r = 49725,
      g = 39585,
      b = 30225,
      w = 32768;

PrintWriter file;

void setup() {
  file = createWriter("../Color_Correction.h");
  float scalar = 0;
  if(r>scalar) scalar = r;
  if(g>scalar) scalar = g;
  if(b>scalar) scalar = b;
  if(w>scalar) scalar = w;
  file.println("#pragma once");
  file.print("#define CORRECTION_PRECISION ");
  file.println((int)(max_out/max_in));
  file.print("#define CORRECTION_R ");
  file.println((int)(r*max_in/scalar));
  file.print("#define CORRECTION_G ");
  file.println((int)(g*max_in/scalar));
  file.print("#define CORRECTION_B ");
  file.println((int)(b*max_in/scalar));
  file.print("#define CORRECTION_W ");
  file.println((int)(w*max_in/scalar));
  addArray("GAMMA_LUT", max_out);
  
  file.flush();
  file.close();
  exit();
}

void addArray(String name, float max)
{
    file.print("const uint16_t ");
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
