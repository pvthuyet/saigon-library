#ifndef FIBO_DEFINE_H_
#define FIBO_DEFINE_H_

#pragma once

#define fistd		std
// use for classes that have both std and std::pmr. such as std::string, std::pmr::string
//++ TODO std::pmr
#define fistdpmr	std
// use for classes only have std::pmr
#define fipmr		std::pmr

#endif // FIBO_DEFINE_H_