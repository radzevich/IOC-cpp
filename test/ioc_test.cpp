#include <gtest/gtest.h>
#include "ioc_container.h"

namespace {

    struct IFirstAbstract {
        [[nodiscard]] virtual int GetInt() const { return 0; };
    };

    struct TFirstConcrete : public IFirstAbstract {
        [[nodiscard]] int GetInt() const override {
            return 1;
        };
    };

    struct TSecondConcrete : public IFirstAbstract {
        [[nodiscard]] int GetInt() const override {
            return 2;
        };
    };

}

TEST(TServiceRegistryTests, RegisterServiceWithoutDependencies) {
    // arrange
    IOC::TServiceRegistry serviceRegistry;

    // act
    serviceRegistry.RegisterFactory<IFirstAbstract, TFirstConcrete>();
    auto instance = serviceRegistry.Get<IFirstAbstract>();

    // assert
    ASSERT_EQ(instance->GetInt(), 1);
}

TEST(TServiceRegistryTests, OverrideRegisteredServiceWithoutDependencies) {
    // arrange
    IOC::TServiceRegistry serviceRegistry;

    // act
    serviceRegistry.RegisterFactory<IFirstAbstract, TFirstConcrete>();
    serviceRegistry.RegisterFactory<IFirstAbstract, TSecondConcrete>();
    auto instance = serviceRegistry.Get<IFirstAbstract>();

    // assert
    ASSERT_EQ(instance->GetInt(), 2);
}