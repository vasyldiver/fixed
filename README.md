# fixed
Class fixed: floating point fast emulation using integer 64 bit arithmetics  ("one-pass" operations, fast processing)

 * Class fixed: floating point fast emulation using integer 64 bit arithmetics  ("one-pass" operations, fast processing)
 * 
 * instead of:
 *
 *   float a, b, c;
 *
 * just write:
 *
 *   fixed a, b, c;
 *
 * Thus, it is enough to change only 'float' (or 'double') to 'fixed' when declaring variables. For the rest, 
 *  no changes are needed in the program (except, possibly, type conversion in some places)
 *
 * Thanks to the features of C ++, operations with variables of the fixed type are performed in the same way 
 *  as with the type float or double, for example:
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
 * На некоторых аппаратных платформах (особенно без математического сопроцессора) целочисленные арифметические операции
 *  выполняются существенно быстрее операций с плавающей запятой
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
