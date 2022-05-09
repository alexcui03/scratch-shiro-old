#ifndef _COROUTINE_H_
#define _COROUTINE_H_

#include <exception>
#include <coroutine>

namespace ccvm {
    class coroutine {
    public:
        class promise_type {
        public:
            std::exception_ptr exception;

            promise_type() noexcept: parent(this), root(this) {}
            std::suspend_always initial_suspend() noexcept {
                return std::suspend_always{};
            }
            std::suspend_always final_suspend() noexcept {
                return std::suspend_always{};
            }
            void unhandled_exception() {
                this->exception = std::current_exception();
            }
            coroutine get_return_object() {
                return coroutine{*this};
            }
            auto yield_value(coroutine &&coro) {
                this->root->parent = coro.promise;
                coro.promise->root = this->root;
                coro.promise->parent = this;
                coro.promise->resume();
                if (!coro.promise->done()) return awaitable_type{coro.promise};
                this->root->parent = this;
                return awaitable_type{nullptr};
            }
            void return_void() {}
            void pop_up() {
                if (this->parent != nullptr) this->parent->resume();
                while (this->parent != this && this->parent->done()) {
                    this->parent = this->parent->parent;
                    this->parent->resume();
                }
            }
            void throw_if_exception () {
                if (this->exception != nullptr)
                    std::rethrow_exception(std::move(this->exception));
            }
            inline void destroy() noexcept {
                std::coroutine_handle<promise_type>::from_promise(*this).destroy();
            }
            inline void resume() noexcept {
                std::coroutine_handle<promise_type>::from_promise(*this).resume();
            }
            inline bool done() noexcept {
                return std::coroutine_handle<promise_type>::from_promise(*this).done();
            }
        private:
            promise_type *parent;
            promise_type *root;
        };

        class awaitable_type {
        public:
            awaitable_type(promise_type *child): child_promise(child) {}
            bool await_ready() noexcept {
                return this->child_promise == nullptr;
            }
            void await_suspend(std::coroutine_handle<promise_type>) noexcept {}
            void await_resume() {
                if (this->child_promise != nullptr)
                    this->child_promise->throw_if_exception();
            }
        private:
            promise_type *child_promise;
        };

        mutable promise_type *promise;

        coroutine() noexcept: promise(nullptr) {}
        coroutine(promise_type &p) noexcept: promise(&p) {};
        coroutine(coroutine &&coro) noexcept: promise(coro.promise) {
            coro.promise = nullptr;
        }
        ~coroutine() {
            if (this->promise != nullptr) this->promise->destroy();
        }
        void resume() {
            this->promise->pop_up();
            if (this->promise->done()) {
                this->promise->throw_if_exception();
            }
        }
        inline bool done() {
            return this->promise->done();
        }
        inline void destroy() {
            this->promise->destroy();
            this->promise = nullptr;
        }
    };

    using promise = coroutine::promise_type;
    using generator_awaitable = coroutine::awaitable_type;
}

#endif // !_COROUTINE_H_
