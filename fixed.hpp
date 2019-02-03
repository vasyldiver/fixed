/*
 * Class fixed: floating point emulation using integer (64 bit) arithmetics
 * 
 *   (each floating point operation is operated by single integer operation)
 *  
 * instead of:
 *
 *   float a, b, c;
 *
 * just write:
 *
 *   fixed a, b, c;
 *
 * Thus, it is enough to change only 'float' (or 'double') to 'fixed' in your code when declaring variables. 
 * For the rest, no changes are needed in the program (except, possibly, type conversion in some places)
 *
 * Thanks to the features of C ++, operations with variables of the fixed type are performed in the same way 
 * as with the type float or double, for example:
 * 
 *   if( (a+b)/c < 0.0 )  { c -= a*b; }
 * 
 * 
 * On some hardware platforms (especially without FPU - math coprocessor), integer arithmetic operations are performed
 *  significantly faster than floating point operations
 * 
 * floating point value is multiplied by 2^24, the integer part is taken and stored as a 64-bit signed integer (int64_t)
 * thus, 40 bits (out of 64) are used for the integer part, and 24 bits for the fractional 
 *  (non-integer in binary representation) part of a real number
 * 
 * That is, the fixed class variables are numbers not with a floating point, but with a fixed point.
 * 
 * when converting back to type float, a 64-bit integer is converted to type float, then divisible by 2^24 (float(1<<24)),
 *  thereby obtaining a floating-point number
 * (if necessary, to convert 'fixed' value to an integer type int, long, etc., it's enough to shift the 64-bit stored integer to the right by 24 bits)
 * 
 * The operations of addition and subtraction of such numbers are performed "in one pass” by adding (subtracting) 64-bit stored integers
 * 
 * Multiply and divide operations are also performed "in one pass" by multiplying and dividing over 64-bit integers, 
 *  but with an additional shift of digits (so the original stored values are already multiplied by 2^24)
 * when processing, 8 bits of accuracy are lost when multiplying and about 6 bits when dividing, there we can assume that
 *  only 16(18) bits of accuracy for the fractional part of the numbers actually "work" when multiplying and dividing
 * 
 * On some hardware platforms (for example, Arduino Uno) the operation of dividing two 64-bit integers takes longer 
 *  (more CPU ticks) than the operation of dividing two floating point values,
 * therefore, to use the floating-point arithmetic for (only) division operation, just uncomment the line:
 * 
 * #define  __fixed_use_float_for_div
 * 
 * If your hardware platform stores in memory floating-point numbers according to the IEEE 754 standard,
 * then multiplication and division by 2^24 when converting from/to type float/double is much faster to produce by adding (subtracting) to(from) the exponent of the number 24
 * To use just such a faster method, uncomment the line:
 * 
 * #define  __fixed_use_fast_float_convertion
 * 
 * 
 * (russian language annotation):
 * 
 * Класс fixed - эмуляция операций с плавающей запятой с помощью целочисленной (64-bit) арифметики 
 * 
 *   (любая арифметическая операция с плавающей точкой эмулируется с помощью ОДНОЙ целочисленной операции) 
 * 
 * вместо:
 * 
 *   float a, b, c;
 * 
 * достаточно написать:
 * 
 *   fixed a, b, c;
 * 
 * Таким образом, достаточно только поменять только 'float' (или 'double') на 'fixed' при объявлении переменных.  
 *  В остальном не нужны никакие изменения в программном (кроме, возможно, приведения типов)
 * 
 * Благодаря возможностям C++, операции с переменными типа fixed производтся так же как и с типом float или double,  например:
 * 
 *   if( (a+b)/c < 0.0 )  { c -= a*b; }
 * 
 * 
 * На некоторых аппаратных платформах (особенно без математического сопроцессора) целочисленные арифметические операции  выполняются существенно быстрее операций с плавающей запятой
 * 
 * вещественнное число с плавающей запятой умножается на 2^24, берётся целая часть и хранится как 64-битное знаковое целое (тип int64_t)
 * таким образом, 40 бит (из 64) используются для целой части, а 24 бита - для дробной (не целой в двоичном представлении) части вещественного числа,
 * то есть переменные класса fixed - это числа не с плавающей запятой, а с фиксированной запятой
 * 
 * при обратном преобразовании в тип float 64-битное целое число приводится к типу float, затем делится на 2^24 (float(1<<24)), тем самым получаем число с плавающей запятой
 * (при необходимости преобразования в целочисленный тип int,long итд, достаточно хранимое 64-битное целое сдвинуть вправо на 24 разряда)
 * 
 * Операции сложения и вычитания таких чисел выполняются в один "проход" путём сложения(вычитания) 64-битных хранимых целых
 * 
 * Операции умножения и деления тоже совершаются "в один проход" с помощью умножения и деления над 64-битными целыми, 
 *  но с дополнительным сдвигом разрядов (так исходные хранимые величины уже умножены на 2^24)
 * при этом теряется 8 бит точности при умножении и около 6 бит при делении, там образом можно считать что реально "работают"
 *  только 16(18) бит точности для дробной части чисел в операциях умножения и деления
 * 
 * На некоторых аппаратных платформах (например, Arduino Uno) операция деления двух 64-битных целых происходит всё же дольше операции деления двух чисел с плаващей запятой,
 * поэтому чтобы операцию деления двух чисел типа 'fixed' производить именно средствами арифметики с плавающей запятой, раскоментируйте строчку:
 * 
 * #define  __fixed_use_float_for_div
 * 
 * Если на Вашей аппаратной платформе числа с плавающей запятой хранятся в памяти согласно стандарта IEEE 754,
 * то умножение и деление на 2^24 при преобразование из/в тип float/double гораздо быстрее производить путём добавления(вычитания) к(из) экспоненте(ы) числа 24
 * Для использования именно такого более быстрого метода раскоментируйте строчку:
 * 
 * #define  __fixed_use_fast_float_convertion
 * 
 * 
 * by Vasyl Ruskykh  (mailto: domanet.adm@gmail.com,  https://www.facebook.com/vasyl.diver)
 * 
 * last updated:  2019-02-02
 * 
 */
  

//#define  __fixed_use_float_for_div
//#define  __fixed_use_fast_float_convertion


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
  inline fixed(float x);    //     { ff = int64_t( x *  float(uint32_t(1)<<24) ); }    // единицу явно указываем 32-разрядной, чтобы при сдвиге влево на 24 разряда не "вылететь" за пределы разрядности
  inline fixed(double x);

  // оператор присваивания 
  //
  inline fixed& operator=(const  fixed   &x)  { ff = x.ff;  return (*this); }    // а нужно ли?

  // унарный минус
  //
  inline fixed operator - ()  { fixed z;  z.ff = -ff;  return z; }  // { return (fixed(0) - (*this)); }
  
  // преобразование к стандартным типам данных
  //
  inline operator double() const;
  inline operator  float() const;
  inline operator    int() const   { register int64_t a = ff;  if(a < 0) { return  -int((-a)>>24);} else { return  int(a>>24);} }
  inline operator   long() const   { register int64_t a = ff;  if(a < 0) { return -long((-a)>>24);} else { return long(a>>24);} }

  // базовые арифметические операции внутри одного типа
  //
  inline fixed operator + (const fixed &x) const;
  inline fixed operator - (const fixed &x) const;
  inline fixed operator * (const fixed &x) const;
  inline fixed operator / (const fixed &x) const;

  // арифметическая операция к самому объекту с тем же типом данных
  //
  inline fixed& operator +=(const fixed &x)  { ff += x.ff;  return (*this); }
  inline fixed& operator -=(const fixed &x)  { ff -= x.ff;  return (*this); }
  inline fixed& operator *=(const fixed &x);
  inline fixed& operator /=(const fixed &x);


  // арифметические операции с типом double/float
  //
  inline fixed operator + (const double &x) const  { return operator + (fixed(x)); }
  inline fixed operator + (const float  &x) const  { return operator + (fixed(x)); }
  inline fixed operator - (const double &x) const  { return operator - (fixed(x)); }
  inline fixed operator - (const float  &x) const  { return operator - (fixed(x)); }
  
  inline fixed& operator +=(const double &x)  { return operator += (fixed(x)); }
  inline fixed& operator +=(const float  &x)  { return operator += (fixed(x)); }
  inline fixed& operator -=(const double &x)  { return operator -= (fixed(x)); }
  inline fixed& operator -=(const float  &x)  { return operator -= (fixed(x)); }
  
#ifdef __fixed_use_fast_float_convertion
  //
  inline fixed operator * (const double &x) const  { return operator * (fixed(x)); }
  inline fixed operator * (const float  &x) const  { return operator * (fixed(x)); }
  inline fixed operator / (const double &x) const  { return operator / (fixed(x)); }
  inline fixed operator / (const float  &x) const  { return operator / (fixed(x)); }

  inline fixed& operator *=(const double &x)  { return operator *=(fixed(x)); }
  inline fixed& operator /=(const double &x)  { return operator /=(fixed(x)); }
  inline fixed& operator *=(const float  &x)  { return operator *=(fixed(x)); }
  inline fixed& operator /=(const float  &x)  { return operator /=(fixed(x)); }
  //
#else
  //
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
  //
#endif
  
  
  // арифметическае операции к самому объекту с другим типом данных, которые можно реализовать быстрее чем через приведение типов (см.умножение)
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
  
#ifdef __fixed_use_float_for_div      
  //
  // на некоторых платформах деление двух 64-битных чисел происходит дольше операции деления с типами float,  поэтому имеет смысл выполнить деление средствами плавающей арифметики
  //
  inline fixed& operator /=(const  int16_t &x)  { ff = float(ff) / float(x);  return (*this); }
  inline fixed& operator /=(const  int32_t &x)  { ff = float(ff) / float(x);  return (*this); }
  inline fixed& operator /=(const  int64_t &x)  { ff = float(ff) / float(x);  return (*this); }
  inline fixed& operator /=(const uint16_t &x)  { ff = float(ff) / float(x);  return (*this); }
  inline fixed& operator /=(const uint32_t &x)  { ff = float(ff) / float(x);  return (*this); }
  inline fixed& operator /=(const uint64_t &x)  { ff = float(ff) / float(x);  return (*this); }
  //
#else
  //
  inline fixed& operator /=(const  int16_t &x)  { if(x !=  int16_t(0)) { ff /= x; return (*this); }  else { return operator /=(fixed(x)); } }
  inline fixed& operator /=(const  int32_t &x)  { if(x !=  int32_t(0)) { ff /= x; return (*this); }  else { return operator /=(fixed(x)); } }   // if division by zero - resolve this problem by 'fixed' class standart method
  inline fixed& operator /=(const  int64_t &x)  { if(x !=  int64_t(0)) { ff /= x; return (*this); }  else { return operator /=(fixed(x)); } }
  inline fixed& operator /=(const uint16_t &x)  { if(x != uint16_t(0)) { ff /= x; return (*this); }  else { return operator /=(fixed(x)); } }
  inline fixed& operator /=(const uint32_t &x)  { if(x != uint32_t(0)) { ff /= x; return (*this); }  else { return operator /=(fixed(x)); } }
  inline fixed& operator /=(const uint64_t &x)  { if(x != uint64_t(0)) { ff /= x; return (*this); }  else { return operator /=(fixed(x)); } }
  //inline fixed& operator /=(const uint64_t &x)  { ff /= x;  return (*this); }
  //
#endif

  
  // некоторые арифметические операции (см.умножение) с другими типами, которые намного быстрее сделать не приводя (не преобразовывая) к типу fixed
  //
  inline fixed operator + (const  int16_t &x) const  { fixed z;  register int64_t a = int64_t(x);  if(a < 0) {z.ff = ff - ((-a)<<24);} else {z.ff = ff + (a<<24);}  return z; }
  inline fixed operator + (const  int32_t &x) const  { fixed z;  register int64_t a = int64_t(x);  if(a < 0) {z.ff = ff - ((-a)<<24);} else {z.ff = ff + (a<<24);}  return z; }
  inline fixed operator + (const  int64_t &x) const  { fixed z;  register int64_t a = int64_t(x);  if(a < 0) {z.ff = ff - ((-a)<<24);} else {z.ff = ff + (a<<24);}  return z; }
  inline fixed operator + (const uint16_t &x) const  { fixed z;  z.ff = ff + int64_t(x) << 24;  return z; }
  inline fixed operator + (const uint32_t &x) const  { fixed z;  z.ff = ff + int64_t(x) << 24;  return z; }
  inline fixed operator + (const uint64_t &x) const  { fixed z;  z.ff = ff + int64_t(x) << 24;  return z; }

  inline fixed operator - (const  int16_t &x) const  { fixed z;  register int64_t a = int64_t(x);  if(a < 0) {z.ff = ff + ((-a)<<24);} else {z.ff = ff - (a<<24);}  return z; }
  inline fixed operator - (const  int32_t &x) const  { fixed z;  register int64_t a = int64_t(x);  if(a < 0) {z.ff = ff + ((-a)<<24);} else {z.ff = ff - (a<<24);}  return z; }
  inline fixed operator - (const  int64_t &x) const  { fixed z;  register int64_t a = int64_t(x);  if(a < 0) {z.ff = ff + ((-a)<<24);} else {z.ff = ff - (a<<24);}  return z; }
  inline fixed operator - (const uint16_t &x) const  { fixed z;  z.ff = ff - int64_t(x) << 24;  return z; }
  inline fixed operator - (const uint32_t &x) const  { fixed z;  z.ff = ff - int64_t(x) << 24;  return z; }
  inline fixed operator - (const uint64_t &x) const  { fixed z;  z.ff = ff - int64_t(x) << 24;  return z; }

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
#else
  //
  inline fixed operator / (const  int16_t &x) const  { fixed z;  if(x !=  int16_t(0)) { z.ff = ff / x; return (*this); }  else { return operator /(fixed(x));  return z; } }   // if division by zero - resolve this problem by 'fixed' class standart method
  inline fixed operator / (const  int32_t &x) const  { fixed z;  if(x !=  int32_t(0)) { z.ff = ff / x; return (*this); }  else { return operator /(fixed(x));  return z; } }
  inline fixed operator / (const  int64_t &x) const  { fixed z;  if(x !=  int64_t(0)) { z.ff = ff / x; return (*this); }  else { return operator /(fixed(x));  return z; } }
  inline fixed operator / (const uint16_t &x) const  { fixed z;  if(x != uint16_t(0)) { z.ff = ff / x; return (*this); }  else { return operator /(fixed(x));  return z; } }
  inline fixed operator / (const uint32_t &x) const  { fixed z;  if(x != uint32_t(0)) { z.ff = ff / x; return (*this); }  else { return operator /(fixed(x));  return z; } }
  inline fixed operator / (const uint64_t &x) const  { fixed z;  if(x != uint64_t(0)) { z.ff = ff / x; return (*this); }  else { return operator /(fixed(x));  return z; } }
  //
#endif


  // операции сравнения
  //
  inline bool operator ==(const fixed &x) const  { return ff == x.ff; }
  inline bool operator !=(const fixed &x) const  { return !operator ==(x); };
  inline bool operator < (const fixed &x) const  { return ff <  x.ff; }
  inline bool operator <=(const fixed &x) const  { return ff <= x.ff; }
  inline bool operator > (const fixed &x) const  { return ff >  x.ff; }
  inline bool operator >=(const fixed &x) const  { return ff >= x.ff; }

  // операции сравнения с типом double/float
  //
  inline bool operator ==(const float &x) const  { return operator ==(fixed(x)); };
  inline bool operator !=(const float &x) const  { return operator !=(fixed(x)); };
  inline bool operator < (const float &x) const  { return operator < (fixed(x)); };
  inline bool operator <=(const float &x) const  { return operator <=(fixed(x)); };
  inline bool operator > (const float &x) const  { return operator > (fixed(x)); };
  inline bool operator >=(const float &x) const  { return operator >=(fixed(x)); };
  
  inline bool operator ==(const double &x) const  { return operator ==(fixed(x)); };
  inline bool operator !=(const double &x) const  { return operator !=(fixed(x)); };
  inline bool operator < (const double &x) const  { return operator < (fixed(x)); };
  inline bool operator <=(const double &x) const  { return operator <=(fixed(x)); };
  inline bool operator > (const double &x) const  { return operator > (fixed(x)); };
  inline bool operator >=(const double &x) const  { return operator >=(fixed(x)); };

  // операции сравнения с целыми числами
  // ...
  //
};


inline fixed::fixed(double x)
{
  //
#ifdef  __fixed_use_fast_float_convertion
  
if(sizeof(double)==8)  // 64 bits
{
  union 
  {
    double    f64;
    uint64_t  i64;
  } z;

  z.f64 = x;
  z.i64 += (const uint64_t)(uint64_t(24) << 52);    // adding 24 to the exponent according IEEE_754 that is equivalent to multiplying by 2^24
                                  // здесь и далее используется конструкция (const uint64_t) чтобы подсказать компилятору посчитать значение на этапе компиляции и использовать уже как 64-битную константу
                                  // potentional bug!  be sure that your floating-point values are less than ~2^100 !!!
  ff = int64_t(z.f64);            // and than convert to integer multiplyed by 2^24 value (using add 24 to the exponent method)
  return;
}
else      // size of double is not 64 bits
{
  ff = int64_t( x * double( (const uint32_t)(uint32_t(1)<<24) ) );    // единицу явно указываем 32-разрядной, чтобы при сдвиге влево на 24 разряда не "вылететь" за пределы разрядности
}
  
#else

  ff = int64_t( x * double( (const uint32_t)(uint32_t(1)<<24) ) );    // multiply using floating-point operation
  
#endif
  //
}


inline fixed::fixed(float x)
{
  //
#ifdef  __fixed_use_fast_float_convertion

if(sizeof(float)==8)  // 64 bits
{
  union 
  {
    float     f64;
    uint64_t  i64;
  } z;

  z.f64 = x;
  z.i64 += (const uint64_t)(uint64_t(24) << 52);    // adding 24 to the exponent according IEEE_754 that is equivalent to multiplying by 2^24

  ff = int64_t(z.f64);            // and than convert to integer multiplyed by 2^24 value (using add 24 to the exponent method)
  return;
}
if(sizeof(float)==4)  // 32 bits
{
  union 
  {
    float     f32;
    uint32_t  i32;
  } z;

  z.f32 = x;
  z.i32 += (const uint32_t)(uint32_t(24) << 23);    // adding 24 to the exponent according IEEE_754 that is equivalent to multiplying by 2^24

  ff = int64_t(z.f32);            // and than convert to integer multiplyed by 2^24 value (using add 24 to the exponent method)
  return;
}
else     // size of float is not 32 or 64 bits!  (а такое бывает?)
{
  ff = int64_t( x * float( (const uint32_t)(uint32_t(1)<<24) ) );    // единицу явно указываем 32-разрядной, чтобы при сдвиге влево на 24 разряда не "вылететь" за пределы разрядности
}

#else

  ff = int64_t( x * float( (const uint32_t)(uint32_t(1)<<24) ) );    // multiply using floating-point operation
  
#endif
  //
}


inline fixed::operator double() const
{
  //
#ifdef  __fixed_use_fast_float_convertion
  
if(sizeof(double)==8)  // 64 bits
{
  union 
  {
    double    f64;
    uint64_t  i64;
  } z;

  z.f64 = double(ff);
  
  register uint64_t a = z.i64;

  if( (a<<1) < (const uint64_t)(uint64_t(25)<<53) )  { return 0.0; }    // return 0 if the shifted exponent less than 25

  z.i64 = a - (const uint64_t)(uint64_t(24) << 52);    // substracting 24 to the exponent according IEEE_754 that is equivalent to dividing by 2^24

  //z.i64 -= uint64_t(24) << 52;    // substracting 24 to the exponent according IEEE_754 that is equivalent to dividing by 2^24

  return z.f64; 
}
else      // size of double is not 64 bits
{
  return double(ff)/double((const uint32_t)(uint32_t(1)<<24)); 
}

#else

  return double(ff)/double((const uint32_t)(uint32_t(1)<<24)); 
  
#endif
  //
}


inline fixed::operator float() const
{
  //
#ifdef  __fixed_use_fast_float_convertion
  
if(sizeof(float)==8)  // 64 bits
{
  union 
  {
    float     f64;
    uint64_t  i64;
  } z;

  z.f64 = float(ff);

  register uint64_t a = z.i64;

  if( (a<<1) < (const uint64_t)(uint64_t(25)<<53) )  { return 0.0; }    // return 0 if the shifted exponent less than 25

  z.i64 = a - (const uint64_t)(uint64_t(24) << 52);    // substracting 24 to the exponent according IEEE_754 that is equivalent to dividing by 2^24

  return z.f64; 
}
if(sizeof(float)==4)  // 32 bits
{
  union 
  {
    float     f32;
    uint32_t  i32;
  } z;

  z.f32 = float(ff);

  register uint32_t a = z.i32;

  if( (a<<1) < (const uint32_t)(uint32_t(25)<<24) )  { return 0.0; }    // return 0 if the shifted exponent less than 25

  z.i32 = a - (const uint32_t)(uint32_t(24) << 23);    // substracting 24 to the exponent according IEEE_754 that is equivalent to dividing by 2^24

  return z.f32; 
}
else      // size of float is not 32 or 64 bits!  (а такое бывает?)
{
  return float(ff)/float((const uint32_t)(uint32_t(1)<<24)); 
}

#else

  return float(ff)/float((const uint32_t)(uint32_t(1)<<24)); 
  
#endif
  //
}


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
  
  a <<= 10;  b >>= 8;
  
  if(b != int64_t(0))
  {
    a /= b;  a <<= 6;
  
    if(sign!=false)  { a = -a; }             // correct sign
  
    z.ff = a;
  }
  else
  {
    if( ff * x.ff >= 0 )
    {
      z.ff = (const int64_t) (( !(uint64_t(0)) ) >> 2);        // just very big positive value
    }
    else
    {
      z.ff = (const int64_t) (-( ( !(uint64_t(0)) ) >> 2));    // just very big negative value
    }
  }
  
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
  
  a <<= 10;  b >>= 8;  
  
  if(b != int64_t(0))
  {
    a /= b;  a <<= 6;
  
    if(sign!=false)  { a = -a; }           // correct sign
  
    ff = a;
  }
  else
  {
    if( ff * x.ff >= 0 )
    {
      ff = (const int64_t) (( !(uint64_t(0)) ) >> 2);        // just very big positive value
    }
    else
    {
      ff = (const int64_t) (-( ( !(uint64_t(0)) ) >> 2));    // just very big negative value
    }
  }
  
#endif
  
  return (*this);
}

