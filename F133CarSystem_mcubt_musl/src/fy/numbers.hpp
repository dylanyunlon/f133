/*
 * numbers.hpp
 *
 *  Created on: 2021年4月7日
 *      Author: pengzc
 */

#ifndef _FY_NUMBERS_HPP_
#define _FY_NUMBERS_HPP_

#include <string.h>
#include <stdint.h>
#include <math.h>

#include <string>

namespace fy {

// https://github.com/zhao520a1a/NumToCN/blob/master/src/NumToCn.java

/**
 * 以分为单位
 */
std::string number_to_currency(int64_t cent) {
  const char* CN_NUMBER[] = { "零", "壹", "贰", "叁", "肆", "伍", "陆", "柒", "捌", "玖" };
  const char* CN_UNIT[] = { "分", "角", "圆", "拾", "佰", "仟", "万", "拾", "佰", "仟",
      "亿", "拾", "佰", "仟", "兆", "拾", "佰", "仟", "顺" };

  const char* CN_NEGATIVE = "负";
  const char* CN_FULL = "整";
  const char* CN_ZERO_FULL = "零圆整";

  std::string currency;

  //若输入为0，输出零圆整；
  if (cent == 0) {
    return CN_ZERO_FULL;
  }

  int64_t number = abs(cent);
  int numIndex = 0; //记录数字的个数；
  bool getZero = false;
  /*
   * 思路：要先判断一下小数部分的具体情况；究其根本是因为：小数部分和整数部分在处理“0”的问题上略有不同；避免出现如图1所示的情况；
   */
  //得到小数部分（小数点后两位）；
  int64_t scale = number % 100;
  if (scale == 0) { //若小数部分为"00"时的情况；骚年，不要忘了在最后追加特殊字符：整
    numIndex += 2;
    getZero = true;
    number /= 100;  // 从number去掉为0数；
    currency.append(CN_FULL);
  } else if (scale % 10 == 0) { //若小数部分为"*0"时的情况；
    numIndex += 1;
    getZero = true;
    number /= 10; // 从number去掉为0数；
  }

  //排除上述两种小数部分的特殊情况，则对小数和整数的处理就是一样一样一样地了！
  while (true) {
    //循环结束条件；
    if (number <= 0) {
      break;
    }

    //每次通过取余来得到最后一位数；
    int numUnit = (int) (number % 10);
    if (numUnit != 0) {
      currency.insert(0, CN_UNIT[numIndex]);  //先添加单位
      currency.insert(0, CN_NUMBER[numUnit]); //在添加根据数字值来对应数组中的中文表述；
      getZero = false; //表明当前数不是0；
    } else {
      //意思是它的上一次的数不是零，那么打印出零；
      if (!getZero) {
        currency.insert(0, CN_NUMBER[numUnit]);
      }
      //若角分位为零，那么打印零；
      if (numIndex == 2) {
        if (number > 0) {
          currency.insert(0, CN_UNIT[numIndex]);
        }
      } else if ((numIndex - 2) % 4 == 0 && number % 1000 != 0) {
        //第一个条件是为了每隔4位，打印“圆，万，亿”；第二个条件是为了避免出现如图3的情况；
        currency.insert(0, CN_UNIT[numIndex]);
      }
      getZero = true; //将其置为true,那么如果下一位还是0,也不会再打印一遍'零'；避免出现图2的情况；
    }

    // 从number每次都去掉最后一个数
    number = number / 10;
    numIndex++;
  }

  // 为负数，就在最前面追加特殊字符：负
  if (cent < 0) {
    currency.insert(0, CN_NEGATIVE);
  }

  return currency;
}

} /* namespace fy */


#endif /* JNI_NUMBERS_HPP_ */
