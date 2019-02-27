#include "InvestmentSystem.h"

#include "components/Bank.h"
#include "components/Investor.h"

void updateInvestmentSystem(Registry &registry, float dt)
{
    registry.view<Bank, Investor>().each([dt](auto entity, Bank &bank, const Investor &investor)
    {
        bank.amount += bank.amount * investor.interests * dt;
    });
}
