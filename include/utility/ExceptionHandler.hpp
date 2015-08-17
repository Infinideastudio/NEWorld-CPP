//
// Copyright 2015 infinideas
//

#ifndef NEWORLD_EXCEPTION_HANDLER_HPP
#define NEWORLD_EXCEPTION_HANDLER_HPP

void SetExcetopnHandler();
void Terminatehandler();

/**
 * C++11中抛弃了unexptected相关的函数
 * 它们也工作不正常
 */
[[deprecated]]
void UnexpectedHandler() noexcept;

#endif  // NEWORLD_EXCEPTION_HANDLER_HPP
