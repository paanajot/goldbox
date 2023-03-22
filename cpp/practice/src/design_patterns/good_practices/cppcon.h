
#pragma once

/* cppcon NOTE: CppCon 2016: Jason Turner “Practical Performance Practices"
   https://www.youtube.com/watch?v=uzF4u9KgUWI&ab_channel=CppCon

 * the smaller the program is, the more likely it is to fit in the cache
 * avoid std::endl
 * avoid shared_ptr, prefer unique_ptr
 * avoid std::function
 * always use const
 * never use std::bind (use lambdas instead)
 * prefer std::array than std::vector
 * avoid automatic conversion - use explicit
*/

/* cppcon NOTE: Back To Basics: Overload Resolution - CppCon 2021
   https://youtu.be/b5Kbzgx1w9A

 * Overload resolution is the process of selecting the most appropriate overload
 * Full list of visible function declarations is called the overload set
 * Process to find the best overload:
    * create the candidate list
    * remove the invalid overloads
    * rank the remaining candidates
    * if exactly one function in the candidate list ranks higher than all others,
      it wins the overload resolution process
    * if there is a tie for the highest ranking, then tie breakers are used,
      e.g. when a template and a non-template candidate are tied for first place,
      the non-template function is selected
*/
