# Crypto Trading Simulator

**Midterm Object Oriented Programming Project**  
Student Name: Rajagopal Raja Reshika  
Student Number: 240621557

A C++ based cryptocurrency trading simulator with a terminal-based user interface, market data analysis, and user management.

## Features
- 🔐 Secure user authentication with password hashing
- 🔒 Account lockout protection (3 failed attempts, 5-minute lockout)
- 💼 Multi-cryptocurrency wallet management
- 📊 Market data analysis and candlestick charts
- 💱 Trading simulation with real-time market data
- 📝 Transaction history tracking
- ⌨️ Interactive arrow-key navigation menu system

## Quick Start

### Compile
```bash
make
```

### Run
```bash
make run
# or
./crypto_sim
```

### Clean
```bash
make clean
```

## Naming Conventions

This project follows a specific naming convention where most classes, functions, and variables are prefixed with `r` and use shortened names for brevity.

### General Rules:
- **Prefix**: `r` is used as a prefix for almost all custom types and members.
- **Shortening**: Words are often shortened (e.g., `Mkt` for Market, `Dat` for Data, `Usr` for User).

---

## Class & Struct Reference

| Short Name | Full Name | Description |

| `rCandle` | Candlestick | Stores Open, High, Low, Close (OHLC) data for a specific timeframe. |
| `rCSV` | CSV Reader | Utility class for reading and writing CSV files. |
| `rMktAnls` | Market Analysis | Logic for computing candlestick data from raw market orders. |
| `rMktDat` | Market Data | Manages the loading and filtering of market orders from `market.csv`. |
| `rMktOrd` | Market Order | Represents a single order (bid or ask) in the market data. |
| `rOrd` | Order | A lightweight struct for representing market orders. |
| `rSim` | Trade Simulator | Core simulation logic, including order generation and user stats. |
| `rTrans` | Transaction | Represents a single financial transaction (deposit, withdrawal, trade). |
| `rTransMgr` | Transaction Manager | Manages the persistence and retrieval of user transactions. |
| `rUsr` | User | Stores user profile information (ID, name, email, hashed password). |
| `rUsrMgr` | User Manager | Handles user registration, login, and wallet management. |
| `rWlt` | Wallet | Tracks cryptocurrency balances for a user. |

---

## Variable & Member Reference

| Short Name | Full Name | Description |

| `rTime` / `rDt` | Timestamp / Date | The date and time of an event (YYYY-MM-DD HH:MM:SS). |
| `rProd` | Product | The trading pair (e.g., ETH/BTC). |
| `rType` / `rTyp` | Type | The type of order or transaction (bid, ask, deposit, etc.). |
| `rPrc` | Price | The price at which an order or transaction occurred. |
| `rAmt` | Amount | The quantity of cryptocurrency involved. |
| `rOp` | Open | The opening price of a candlestick. |
| `rHi` | High | The highest price in a candlestick period. |
| `rLo` | Low | The lowest price in a candlestick period. |
| `rCl` | Close | The closing price of a candlestick. |
| `rUsrNm` | Username | The unique 10-digit ID assigned to a user. |
| `rFullNm` | Full Name | The user's real name. |
| `rEml` | Email | The user's email address. |
| `rPwdHsh` | Password Hash | The hashed version of the user's password. |
| `rBalAft` | Balance After | The wallet balance after a transaction. |
| `rFn` | Filename | The path to a file. |
| `rTf` | Timeframe | The period for analysis (yearly, monthly, daily). |

---

## Function Reference

### `rCSV` (CSV Utilities)
- `rRdCSV`: Read CSV (Generic)
- `rRdMkt`: Read Market Data
- `rAppRow`: Append Row
- `rWrtAll`: Write All
- `rSpl`: Split String
- `rTime`: Get Current Timestamp

### `rMktDat` (Market Data)
- `rGetOrds`: Get Orders
- `rGetProds`: Get Products
- `rGenCndls`: Generate Candlesticks
- `rCompCndls`: Compute Candlesticks (Static)
- `rBktKy`: Get Bucket Key (Static)

### `rUsrMgr` (User Management)
- `rRegUsr`: Register User
- `rLgnUsr`: Login User
- `rFndUsr`: Find User
- `rRstPwd`: Reset Password
- `rGetWlt`: Get Wallet
- `rSavWlts`: Save Wallets

### `rSim` (Simulation)
- `rGenOrds`: Generate Random Orders
- `rGetStats`: Get User Statistics
- `rSimTrds`: Simulate Automated Trades

### `rUI` (UI Utilities Namespace)
- `rLogo`: Print Application Logo
- `rMenu`: Display Interactive Menu
- `rHdr`: Print Section Header
- `rOk`: Print Success Message
- `rErr`: Print Error Message
- `rInf`: Print Info Message
- `rWrn`: Print Warning Message

---

## File Structure

- `src/`: Contains all source code (`.cpp` and `.h` files).
- `data/`: Contains CSV files for persistence (`users.csv`, `wallets.csv`, `transactions.csv`, `market.csv`).
- `Rcrypto_sim`: The compiled executable.
