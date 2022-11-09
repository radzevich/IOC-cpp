#include <gtest/gtest.h>
#include "ioc_container.h"

namespace {

    struct IFirstAbstract {
        [[nodiscard]] virtual int GetInt() const { return -1; };
    };

    struct ISecondAbstract {
        [[nodiscard]] virtual int GetInt() const { return -2; };
    };

    struct TFirstConcrete : public IFirstAbstract {
        [[nodiscard]] int GetInt() const override {
            return 1;
        };
    };

    struct TAnotherFirstConcrete : public IFirstAbstract {
        [[nodiscard]] int GetInt() const override {
            return 11;
        };
    };

    struct TSecondConcrete : public ISecondAbstract {
        [[nodiscard]] int GetInt() const override {
            return 2;
        };
    };

    struct IComplexAbstract {
        [[nodiscard]] virtual int GetInt() const { return -3; };
    };

    struct TComplexConcrete : public IComplexAbstract {
        TComplexConcrete(std::shared_ptr<IFirstAbstract> first, std::shared_ptr<ISecondAbstract> second)
            : First(std::move(first))
            , Second(std::move(second))
        {
        }

        [[nodiscard]] int GetInt() const override {
            return 3;
        };

    private:
        std::shared_ptr<IFirstAbstract> First;
        std::shared_ptr<ISecondAbstract> Second;
    };

}

TEST(TServiceRegistryTestsWithoutDependencies, RegisterSingleService) {
    // arrange
    IOC::TServiceRegistry serviceRegistry;

    // act
    serviceRegistry.RegisterFactory<IFirstAbstract, TFirstConcrete>();
    auto instance = serviceRegistry.Get<IFirstAbstract>();

    // assert
    ASSERT_EQ(instance->GetInt(), 1);
}

TEST(TServiceRegistryTestsWithoutDependencies, RegisterSeveralServices) {
    // arrange
    IOC::TServiceRegistry serviceRegistry;

    // act
    serviceRegistry.RegisterFactory<IFirstAbstract, TFirstConcrete>();
    serviceRegistry.RegisterFactory<ISecondAbstract, TSecondConcrete>();
    auto instance1 = serviceRegistry.Get<IFirstAbstract>();
    auto instance2 = serviceRegistry.Get<ISecondAbstract>();

    // assert
    ASSERT_EQ(instance1->GetInt(), 1);
    ASSERT_EQ(instance2->GetInt(), 2);
}

TEST(TServiceRegistryTestsWithoutDependencies, OverrideRegisteredService) {
    // arrange
    IOC::TServiceRegistry serviceRegistry;

    // act
    serviceRegistry.RegisterFactory<IFirstAbstract, TFirstConcrete>();
    serviceRegistry.RegisterFactory<IFirstAbstract, TAnotherFirstConcrete>();
    auto instance = serviceRegistry.Get<IFirstAbstract>();

    // assert
    ASSERT_EQ(instance->GetInt(), 11);
}

TEST(TServiceRegistryTestsWithoutDependencies, RegisterServiceAsSelf) {
    // arrange
    IOC::TServiceRegistry serviceRegistry;

    // act
    serviceRegistry.RegisterFactory<TFirstConcrete, TFirstConcrete>();
    auto instance = serviceRegistry.Get<TFirstConcrete>();

    // assert
    ASSERT_EQ(instance->GetInt(), 1);
}

TEST(TServiceRegistryTestsWithDependencies, RegisterComplexAndDependencies) {
    // arrange
    IOC::TServiceRegistry serviceRegistry;

    // act
    serviceRegistry.RegisterFactory<IFirstAbstract, TFirstConcrete>();
    serviceRegistry.RegisterFactory<ISecondAbstract, TSecondConcrete>();
    serviceRegistry.RegisterFactory<IComplexAbstract, TComplexConcrete, IFirstAbstract, ISecondAbstract>();

    auto instance = serviceRegistry.Get<IComplexAbstract>();

    // assert
    ASSERT_EQ(instance->GetInt(), 3);
}