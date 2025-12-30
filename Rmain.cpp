#include "RMenu.h"
#include "RUserManager.h"
#include "RTransactionManager.h"
#include "RMarketData.h"
#include "RTradeSimulator.h"

int main() {
    MarketData marketData("market.csv");
    UserManager userManager("users.csv", "wallets.csv");
    TransactionManager transactionManager("transactions.csv");
    TradeSimulator tradeSimulator(marketData);

    Menu menu(userManager, transactionManager, tradeSimulator);
    menu.init();

    return 0;
}
