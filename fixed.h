/*
 * Class fixed - floating point emulation using integer (64-bit) arithmetic
 * 
 * On some hardware platforms (especially without FPU - math coprocessor), integer arithmetic operations are performed significantly faster than floating point operations
 * 
 * floating point value is multiplied by 2^24, the integer part is taken and stored as a 64-bit signed integer (int64_t)
 * thus it can be considered that 40 bits (out of 64) are used for the integer part, and 24 bits for the fractional (non-integer in binary representation) part of a real number
 * 
 * when converting back to type float, a 64-bit integer is converted to type float, then divisible by 2^24 (float(1<<24)), thereby obtaining a floating-point number
 * (if necessary, to convert 'fixed' value to an integer type int, long, etc., it's enough to shift the 64-bit stored integer to the right by 24 bits)
 * 
 * The operations of addition and subtraction of such numbers are performed "in one pass” by adding (subtracting) 64-bit stored integers
 * 
 * Multiply and divide operations are also performed "in one pass" by multiplying and dividing over 64-bit integers, but with an additional shift of digits (so the original stored values are already multiplied by 2^24)
 * when processing, 8 bits of accuracy are lost when multiplying and about 6 bits when dividing, there we can assume that only 16(18) bits of accuracy for the fractional part of the numbers actually "work" when multiplying and dividing
 * 
 * On some hardware platforms (for example, Arduino Uno) the operation of dividing two 64-bit integers takes longer (more CPU ticks) than the operation of dividing two floating point values,
 * therefore, to use the floating-point arithmetic for (only) division operation, just uncomment the line:
 * 
 * #define  __fixed_use_float_for_div
 * 
 * 
 * (russian language annotation):
 * 
 * Класс fixed - эмуляция операций с плавающей запятой с помощью целочисленной (64-bit) арифметики 
 * 
 * На некоторых аппаратных платформах (особенно без математического сопроцессора) целочисленные арифметические операции выполняются существенно быстрее операций с плавающей запятой
 * 
 * вещественнное число с плавающей запятой умножается на 2^24, берётся целая часть и хранится как 64-битное знаковое целое (тип int64_t)
 * таким образом можно считать что 40 бит (из 64) используются для целой части, а 24 бита - для дробной (не целой в двоичном представлении) части вещественного числа
 * 
 * при обратном преобразовании в тип float 64-битное целое число приводится к типу float, затем делится на 2^24 (float(1<<24)), тем самым получаем число с плавающей запятой
 * (при необходимости преобразования в целочисленный тип int,long итд, достаточно хранимое 64-битное целое сдвинуть вправо на 24 разряда)
 * 
 * Операции сложения и вычитания таких чисел выполняются в один "проход" путём сложения(вычитания) 64-битных хранимых целых
 * 
 * Операции умножения и деления тоже совершаются "в один проход" с помощью умножения и деления над 64-битными целыми, но с дополнительным сдвигом разрядов (так исходные хранимые величины уже умножены на 2^24)
 * при этом теряется 8 бит точности при умножении и около 6 бит при делении, там образом можно считать что реально "работают" только 16(18) бит точности для дробной части чисел в операциях умножения и деления
 * 
 * На некоторых аппаратных платформах (например, Arduino Uno) операция деления двух 64-битных целых происходит всё же дольше операции деления двух чисел с плаващей запятой,
 * поэтому чтобы операцию деления двух чисел типа 'fixed' производить именно средствами арифметики с плавающей запятой, раскоментируйте строчку:
 * 
 * #define  __fixed_use_float_for_div
 * 
 * 
 * by Vasyl Ruskykh  (mailto: domanet.adm@gmail.com,  https://www.facebook.com/vasyl.diver)
 * 
 * last updated:  2019-02-01
 * 
 */
 

//#define  __fixed_use_float_for_div


class fixed
{
  int64_t ff;

public:
  // конструкторы
  //
  inline fixed()            { ff = 0; }
  inline fixed(int8_t  x)   { register int64_t a = int64_t(x);  ff = (a < 0) ? -((-a)<<24) : (a<<24); }
  inline fixed(int16_t x)   { register int64_t a = int64_t(x);  ff = (a < 0) ? -((-a)<<24) : (a<<24); }
  inline fixed(int32_t x)   { register int64_t a = int64_t(x);  ff = (a < 0) ? -((-a)<<24) : (a<<24); }
  inline fixed(int64_t x)   { register int64_t a = int64_t(x);  ff = (a < 0) ? -((-a)<<24) : (a<<24); }
  inline fixed(uint8_t x)   { ff = int64_t(x) << 24; }
  inline fixed(uint16_t x)  { ff = int64_t(x) << 24; }
  inline fixed(uint32_t x)  { ff = int64_t(x) << 24; }
  inline fixed(uint64_t x)  { ff = int64_t(x) << 24; }
  inline fixed(float x)     { ff = int64_t( x *  float(uint32_t(1)<<24) ); }    // единицу явно указываем 32-разрядной, чтобы при сдвиге влево на 24 разряда не "вылететь" за пределы разрядности
  inline fixed(double x)    { ff = int64_t( x * double(uint32_t(1)<<24) ); }

  // оператор присваивания 
  //
  inline fixed& operator=(const  fixed   &x)  { ff = x.ff;  return (*this); }    // а нужно ли?

  // унарный минус
  //
  inline fixed operator - ()  { fixed z;  z.ff = -ff;  return z; }  // { return (fixed(0) - (*this)); }
  
  // преобразование к стандартным типам данных
  //
  inline operator float() const   { return float(ff)/float(uint32_t(1)<<24); }
  inline operator int()   const   { register int64_t a = ff;  if(a < 0) { return  -int((-a)>>24);} else { return  int(a>>24);} }
  inline operator long()  const   { register int64_t a = ff;  if(a < 0) { return -long((-a)>>24);} else { return long(a>>24);} }

  // базовые арифметические операции внутри одного типа
  //
  inline fixed operator + (const fixed &x) const;
  inline fixed operator - (const fixed &x) const;
  inline fixed operator * (const fixed &x) const;
  inline fixed operator / (const fixed &x) const;

  // некоторые арифметические операции (умножение) с другими типами, которые намного быстрее сделать не приводя (не преобразовывая) к типу fixed
  //
  inline fixed operator * (const  int16_t &x) const  { fixed z;  z.ff = ff * x;  return z; }
  inline fixed operator * (const  int32_t &x) const  { fixed z;  z.ff = ff * x;  return z; }
  inline fixed operator * (const  int64_t &x) const  { fixed z;  z.ff = ff * x;  return z; }
  inline fixed operator * (const uint16_t &x) const  { fixed z;  z.ff = ff * x;  return z; }
  inline fixed operator * (const uint32_t &x) const  { fixed z;  z.ff = ff * x;  return z; }
  inline fixed operator * (const uint64_t &x) const  { fixed z;  z.ff = ff * x;  return z; }

#ifdef __fixed_use_float_for_div      
  //
  // на некоторых платформах деление двух 64-битных чисел происходит дольше операции деления с типами float,  поэтому имеет смысл выполнить деление средствами плавающей арифметики
  //
  inline fixed operator / (const  int16_t &x) const  { fixed z;  z.ff = float(ff) / float(x);  return z; }
  inline fixed operator / (const  int32_t &x) const  { fixed z;  z.ff = float(ff) / float(x);  return z; }
  inline fixed operator / (const  int64_t &x) const  { fixed z;  z.ff = float(ff) / float(x);  return z; }
  inline fixed operator / (const uint16_t &x) const  { fixed z;  z.ff = float(ff) / float(x);  return z; }
  inline fixed operator / (const uint32_t &x) const  { fixed z;  z.ff = float(ff) / float(x);  return z; }
  inline fixed operator / (const uint64_t &x) const  { fixed z;  z.ff = float(ff) / float(x);  return z; }
  //
#endif
  
  // умножение с типом float/double быстрее сделать средствами арифметики с плавающей запятой, потому как при приведении к типу fixed используется операция умножения двух типов float*float
  //
  inline fixed operator * (const float  &x) const  { fixed z;  z.ff = float(ff) * float(x);  return z; }
  inline fixed operator * (const double &x) const  { fixed z;  z.ff = float(ff) * float(x);  return z; }
  //
  inline fixed& operator *=(const float  &x) { ff = float(ff) * float(x);  return (*this); }
  inline fixed& operator *=(const double &x) { ff = float(ff) * float(x);  return (*this); }
  
  // деление с типом float/double однозначно быстрее сделать средствами арифметики с плавающей запятой, потому как при приведении к типу fixed используется операция умножения двух типов float*float  и  деление двух 64-битных чисел происходит дольше операции деления с типами float
  //
  inline fixed operator / (const float  &x) const  { fixed z;  z.ff = float(ff) / float(x);  return z; }
  inline fixed operator / (const double &x) const  { fixed z;  z.ff = float(ff) / float(x);  return z; }
  //
  inline fixed& operator /=(const float  &x) { ff = float(ff) / float(x);  return (*this); }
  inline fixed& operator /=(const double &x) { ff = float(ff) / float(x);  return (*this); }

  
  // арифметическая операция к самому объекту с тем же типом данных
  //
  inline fixed& operator +=(const fixed &x)  { ff += x.ff;  return (*this); }
  inline fixed& operator -=(const fixed &x)  { ff -= x.ff;  return (*this); }
  inline fixed& operator *=(const fixed &x);
  inline fixed& operator /=(const fixed &x);

  // арифметическая операция к самому объекту с другим типом данных, которую можно реализовать быстрее чем через приведение типов
  //
  inline fixed& operator +=(const  int16_t &x)  { register int64_t a = int64_t(x);  if(a < 0) {ff -= ((-a)<<24);} else {ff += (a<<24);}  return (*this); }    // { ff += (x < 0) ? -(int64_t(-x)<<24) : (int64_t(x)<<24); }
  inline fixed& operator +=(const  int32_t &x)  { register int64_t a = int64_t(x);  if(a < 0) {ff -= ((-a)<<24);} else {ff += (a<<24);}  return (*this); }
  inline fixed& operator +=(const  int64_t &x)  { register int64_t a = int64_t(x);  if(a < 0) {ff -= ((-a)<<24);} else {ff += (a<<24);}  return (*this); }
  inline fixed& operator +=(const uint16_t &x)  { ff += int64_t(x) << 24;  return (*this); }
  inline fixed& operator +=(const uint32_t &x)  { ff += int64_t(x) << 24;  return (*this); }
  inline fixed& operator +=(const uint64_t &x)  { ff += int64_t(x) << 24;  return (*this); }
  
  inline fixed& operator -=(const  int16_t &x)  { register int64_t a = int64_t(x);  if(a < 0) {ff += ((-a)<<24);} else {ff -= (a<<24);}  return (*this); }
  inline fixed& operator -=(const  int32_t &x)  { register int64_t a = int64_t(x);  if(a < 0) {ff += ((-a)<<24);} else {ff -= (a<<24);}  return (*this); }
  inline fixed& operator -=(const  int64_t &x)  { register int64_t a = int64_t(x);  if(a < 0) {ff += ((-a)<<24);} else {ff -= (a<<24);}  return (*this); }
  inline fixed& operator -=(const uint16_t &x)  { ff -= int64_t(x) << 24;  return (*this); }
  inline fixed& operator -=(const uint32_t &x)  { ff -= int64_t(x) << 24;  return (*this); }
  inline fixed& operator -=(const uint64_t &x)  { ff -= int64_t(x) << 24;  return (*this); }
  
  inline fixed& operator *=(const  int16_t &x)  { ff *= x;  return (*this); }
  inline fixed& operator *=(const  int32_t &x)  { ff *= x;  return (*this); }
  inline fixed& operator *=(const  int64_t &x)  { ff *= x;  return (*this); }
  inline fixed& operator *=(const uint16_t &x)  { ff *= x;  return (*this); }
  inline fixed& operator *=(const uint32_t &x)  { ff *= x;  return (*this); }
  inline fixed& operator *=(const uint64_t &x)  { ff *= x;  return (*this); }
  
  inline fixed& operator /=(const  int16_t &x)  { ff /= x;  return (*this); }
  inline fixed& operator /=(const  int32_t &x)  { ff /= x;  return (*this); }
  inline fixed& operator /=(const  int64_t &x)  { ff /= x;  return (*this); }
  inline fixed& operator /=(const uint16_t &x)  { ff /= x;  return (*this); }
  inline fixed& operator /=(const uint32_t &x)  { ff /= x;  return (*this); }
  inline fixed& operator /=(const uint64_t &x)  { ff /= x;  return (*this); }
  
  // операции сравнения
  //
  inline bool operator ==(const fixed &x) const  { return ff == x.ff; }
  inline bool operator !=(const fixed &x) const  { return !operator ==(x); };
  inline bool operator < (const fixed &x) const  { return ff <  x.ff; }
  inline bool operator <=(const fixed &x) const  { return ff <= x.ff; }
  inline bool operator > (const fixed &x) const  { return ff >  x.ff; }
  inline bool operator >=(const fixed &x) const  { return ff >= x.ff; }
  //
};


inline fixed fixed::operator + (const fixed &x) const
{
  fixed z;
  
  z.ff = ff + x.ff; 
  
  return z;
}


inline fixed fixed::operator - (const fixed &x) const
{
  fixed z;
  
  z.ff = ff - x.ff; 
  
  return z;
}


inline fixed fixed::operator * (const fixed &x) const
{
  fixed z;
  
  register bool sign = false;
  register int64_t a, b;

  a = ff;  b = x.ff;
  
  if(a < 0)  { a = -a;  sign ^= 1; }    // "sign ^= 1"  should be more faster than  "sign = !sign"
  if(b < 0)  { b = -b;  sign ^= 1; }    // now a and b are positive
  
  //y.ff = (ff>>12) * (x.ff>>12); 
  //y.ff = ((ff>>8) * (x.ff>>8)) >> 8;      // умножаем сдвинутые именно на 8 (а не 24/2=12) разрядов оба числа, чтобы обеспечить бОльшую точность, после чего еше сдвигаем на 8 разрядов, чтобы в итоге получить сдвиг вправо на 24 разряда

  a >>= 8;  b >>= 8;  a *= b;  a >>= 8;

  if(sign!=false)  { a = -a; }          // correct sign

  z.ff = a;
  
  return z;
}


inline fixed& fixed::operator *= (const fixed &x) 
{
  register bool sign = false;
  register int64_t a, b;

  a = ff;  b = x.ff;
  
  if(a < 0)  { a = -a;  sign ^= 1; }    // "sign ^= 1"  should be more faster than  "sign = !sign"
  if(b < 0)  { b = -b;  sign ^= 1; }    // now a and b are positive
  
  // ff = ((ff>>8) * (x.ff>>8)) >> 8; 

  a >>= 8;  b >>= 8;  a *= b;  a >>= 8;

  if(sign!=false)  { a = -a; }          // correct sign

  ff = a;   return (*this);
}


inline fixed fixed::operator / (const fixed &x) const
{
  fixed z;

#ifdef __fixed_use_float_for_div      
  
  z.ff = int64_t(float(ff)/float(x));     // деление делаем средствами плавающей арифметики - это быстрее чем деление 64-рязрядных чисел
  
#else
  
  register bool sign = false;
  register int64_t a, b;

  a = ff;  b = x.ff;
  
  if(a < 0)  { a = -a;  sign ^= 1; }    // "sign ^= 1"  should be more faster than  "sign = !sign"
  if(b < 0)  { b = -b;  sign ^= 1; }    // now a and b are positive
  
  //y.ff = ((ff<<8) / (x.ff>>8)) << 8; 
  //y.ff = ((ff<<10) / (x.ff>>8)) << 6;
  
  a <<= 10;  b >>= 8;  a /= b;  a <<= 6;
  
  if(sign!=false)  { a = -a; }          // correct sign
  
  z.ff = a;
  
#endif
  
  return z;
}


inline fixed& fixed::operator /= (const fixed &x) 
{
  //
#ifdef __fixed_use_float_for_div      
  
  ff = int64_t(float(ff)/float(x));     // деление делаем средствами плавающей арифметики - это быстрее чем деление 64-рязрядных чисел
  
#else
  
  register bool sign = false;
  register int64_t a, b;

  a = ff;  b = x.ff;
  
  if(a < 0)  { a = -a;  sign ^= 1; }    // "sign ^= 1"  should be more faster than  "sign = !sign"
  if(b < 0)  { b = -b;  sign ^= 1; }    // now a and b are positive
  
  //y.ff = ((ff<<8) / (x.ff>>8)) << 8; 
  //y.ff = ((ff<<10) / (x.ff>>8)) << 6;
  
  a <<= 10;  b >>= 8;  a /= b;  a <<= 6;
  
  if(sign!=false)  { a = -a; }          // correct sign
  
  ff = a;
  
#endif
  
  return (*this);
}


