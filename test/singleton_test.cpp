#include <atomic>
#include <catch2/catch_all.hpp>
#include <iostream>
#include <samwarring/singleton.hpp>
#include <sstream>
#include <thread>
#include <vector>

using namespace samwarring;

// For testing the `singleton` function.
class singleton_test_class {
  public:
    singleton_test_class() noexcept {
        instances_++;
    }
    singleton_test_class(const singleton_test_class&) = delete;
    singleton_test_class(singleton_test_class&&) = delete;

    ~singleton_test_class() {
        instances_--;
    }

    static int instances() noexcept {
        return instances_;
    }

  private:
    static int instances_;
};

int singleton_test_class::instances_{0};

// For testing the `reference_counted_singleton` function.
class rc_singleton_test_class {
  public:
    rc_singleton_test_class() noexcept {
        instances_++;
    }
    rc_singleton_test_class(const rc_singleton_test_class&) = delete;
    rc_singleton_test_class(rc_singleton_test_class&&) = delete;

    ~rc_singleton_test_class() {
        instances_--;
    }

    static int instances() noexcept {
        return instances_;
    }

  private:
    static int instances_;
};

int rc_singleton_test_class::instances_{0};

class singleton_tag_x {};

// The nature of `singleton` is that once obtained, the object remains alive for
// the duration of the entire program and cannot be destroyed. This breaks the
// guarantees of many test frameworks (including Catch2), which try to tear down
// after every test.
TEST_CASE("singleton") {
    REQUIRE(singleton_test_class::instances() == 0);

    auto& default1 = singleton<singleton_test_class>();
    auto& default2 = singleton<singleton_test_class>();
    REQUIRE(singleton_test_class::instances() == 1);
    REQUIRE(&default1 == &default2);

    auto& x1 = singleton<singleton_test_class, singleton_tag_x>();
    auto& x2 = singleton<singleton_test_class, singleton_tag_x>();
    REQUIRE(singleton_test_class::instances() == 2);
    REQUIRE(&x1 == &x2);
    REQUIRE(&x1 != &default1); // Different tags => different singletons.
}

// The nature of `reference_counted_singleton` is that the singleton gets
// destroyed when the last reference to it goes away. This makes it much more
// cooperative in unit-testing.
TEST_CASE("reference counted singleton") {
    REQUIRE(rc_singleton_test_class::instances() == 0);

    {
        auto rc1 = reference_counted_singleton<rc_singleton_test_class>();
        auto rc2 = reference_counted_singleton<rc_singleton_test_class>();
        REQUIRE(rc_singleton_test_class::instances() == 1);
        REQUIRE(rc1 == rc2);

        auto rcx1 = reference_counted_singleton<rc_singleton_test_class,
                                                singleton_tag_x>();
        auto rcx2 = reference_counted_singleton<rc_singleton_test_class,
                                                singleton_tag_x>();
        REQUIRE(rc_singleton_test_class::instances() == 2);
        REQUIRE(rcx1 == rcx2);
        REQUIRE(rcx1 != rc1);
    }

    SECTION("all references gone, so is singleton object") {
        REQUIRE(rc_singleton_test_class::instances() == 0);
    }
}

class multi_threaded_singleton {
  public:
    multi_threaded_singleton() {
        int prev_instances = instances_.fetch_add(1);
        if (prev_instances != 0) {
            std::ostringstream msg;
            msg << "Constructed " << (prev_instances + 1) << " instances";
            throw std::runtime_error{msg.str()};
        }
    }

    ~multi_threaded_singleton() {
        instances_.fetch_sub(1);
    }

    multi_threaded_singleton(const multi_threaded_singleton&) = delete;
    multi_threaded_singleton(multi_threaded_singleton&&) = delete;

  private:
    static std::atomic<int> instances_;
};

std::atomic<int> multi_threaded_singleton::instances_{0};

TEST_CASE("threaded reference counted singleton") {
    std::vector<std::thread> threads;
    const std::size_t NUM_THREADS = 4;
    const std::size_t NUM_ITERS = 10000;
    for (std::size_t i = 0; i < NUM_THREADS; ++i) {
        threads.emplace_back([=] {
            for (std::size_t j = 0; j < NUM_ITERS; ++j) {
                // obtain reference.
                auto handle =
                    reference_counted_singleton<multi_threaded_singleton>();

                // release reference.
                handle.reset();

                // At no point should we ever create more than 1 instance.
                // Otherwise, the singleton property did not hold, and the
                // `multi_threaded_singleton` constructor/destructor will fail.
            }
        });
    }
    for (auto& t : threads) {
        t.join();
    }
}