#ifndef DEFINE_H_
#define DEFINE_H_

#pragma once

#define fistd		std
// use for classes that have both std and std::pmr. such as std::string, std::pmr::string
#define fistdpmr	std::pmr
// use for classes only have std::pmr
#define fipmr		std::pmr

#endif // DEFINE_H_