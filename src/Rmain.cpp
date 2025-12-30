#include "RMenu.h"
#include "RUserManager.h"
#include "RTransactionManager.h"
#include "RMarketData.h"
#include "RTradeSimulator.h"

int main() {
    MarketData marketData("data/20200317.csv");
    UserManager userManager("data/users.csv", "data/wallets.csv");
    TransactionManager transactionManager("data/transactions.csv");
    TradeSimulator tradeSimulator(marketData);

    Menu menu(userManager, transactionManager, tradeSimulator);
    menu.init();

    return 0;
}
