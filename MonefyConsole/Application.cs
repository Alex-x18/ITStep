using System;
using System.Collections.Generic;
using System.Linq;

namespace MonefyConsole {
	static class Application {
		static Application() {
			AccountManager = new AccountManager("AccountsData.json");
			AccountManager.LoadData();
			AccountActionManager = null;
			if (AccountManager.Data.Count == 0) {
				CurrentAccount = null;
				AccountActionManager = null;
			} else {
				CurrentAccount = AccountManager.Data[0];
				AccountActionManager = new AccountActionManager(CurrentAccount.Id);
				AccountActionManager.LoadData();
			}
		}
		private static AccountManager AccountManager { get; }
		private static AccountActionManager AccountActionManager { get; set; }
		private static Account CurrentAccount { get; set; }
		public static bool IsAccountActive() => CurrentAccount != null;
		public static ApplicationResult GetAllAccountTypes(out IReadOnlyList<string> types) {
			if (AccountManager.Data.Count == 0) {
				types = null;
				return new ApplicationResult {
					Success = false,
					Message = "Cannot obtain account types"
				};
			}
			types = AccountManager.Data.Select(acc => acc.Type).ToList();
			return new ApplicationResult {
				Success = true,
				Message = string.Empty
			};
		}
		public static ApplicationResult GetCurrentAccountInfo(out string info) {
			if (CurrentAccount == null) {
				info = null;
				return new ApplicationResult {
					Success = false,
					Message = "No active account"
				};
			}
			info = $"{CurrentAccount.Type}: {CurrentAccount.Currency}, {CurrentAccount.Name}";
			return new ApplicationResult {
				Success = true,
				Message = string.Empty
			};
		}
		public static ApplicationResult SwitchAccount(string targetAccountType) {
			if (CurrentAccount != null && targetAccountType == CurrentAccount.Type) {
				return new ApplicationResult {
					Success = false,
					Message = $"{targetAccountType} is already chosen"
				};
			}
			var result = AccountManager.Data.FirstOrDefault(acc => acc.Type == targetAccountType);
			if (result == null) {
				return new ApplicationResult {
					Success = false,
					Message = $"Type of {targetAccountType} does not exist"
				};
			}
			AccountActionManager?.Dispose();
			CurrentAccount = result;
			AccountActionManager = new AccountActionManager(CurrentAccount.Id);
			AccountActionManager.LoadData();
			return new ApplicationResult {
				Success = true,
				Message = "Successfully switched"
			};
		}
		public static ApplicationResult ModifyAccount(string newAccountCurrencyType = null, string newAccountType = null) {
			if (CurrentAccount == null) {
				return new ApplicationResult {
					Success = false,
					Message = "No active accounts"
				};
			}
			if (string.IsNullOrEmpty(newAccountCurrencyType) && string.IsNullOrEmpty(newAccountType)) {
				return new ApplicationResult {
					Success = false,
					Message = "At least one argument must be non-null"
				};
			}
			if (newAccountCurrencyType != null && newAccountCurrencyType != CurrentAccount.Currency.Type) {
				if (!AccountCurrency.Exists(newAccountCurrencyType)) {
					return new ApplicationResult {
						Success = false,
						Message = $"Currency {newAccountCurrencyType} is not supported"
					};
				}
				CurrentAccount.Currency.Type = newAccountCurrencyType;
				foreach (var item in AccountActionManager.Data) {
					item.Currency.Type = newAccountCurrencyType;
				}
				AccountActionManager.UpdateData();
			}
			if (newAccountType != null && newAccountType != CurrentAccount.Type) {
				if (AccountManager.Data.Exists(acc => acc.Type == newAccountType)) {
					return new ApplicationResult {
						Success = false,
						Message = $"Account {newAccountType} already exists"
					};
				}
				CurrentAccount.Type = newAccountType;
			}
			AccountManager.UpdateData();
			return new ApplicationResult {
				Success = true,
				Message = "Successfully modified the account"
			};
		}
		public static ApplicationResult CreateAccount(string alias, string currencyType, string accountType, decimal amount = 0) {
			if (AccountManager.Data.Exists(account => account.Type == accountType)) {
				return new ApplicationResult {
					Success = false,
					Message = "Account with such name already exists"
				};
			}
			if (!AccountCurrency.Exists(currencyType)) {
				return new ApplicationResult {
					Success = false,
					Message = $"Currency {currencyType} is not supported"
				};
			}
			AccountManager.AddAccount(new AccountCurrency(currencyType, amount), accountType, alias);
			AccountManager.UpdateData();
			return new ApplicationResult {
				Success = true,
				Message = $"Account successfully created"
			};
		}
		public static ApplicationResult TransferToAccount(string accountType, decimal amount) {
			if (CurrentAccount.Type == accountType) {
				return new ApplicationResult {
					Success = false,
					Message = "Cannot transfer to yourself"
				};
			}
			if (CurrentAccount.Currency.Amount <= 0) {
				return new ApplicationResult {
					Success = false,
					Message = "Insuffisent amount on the account"
				};
			}
			if (amount <= 0) {
				return new ApplicationResult {
					Success = false,
					Message = "Amount cannot be zero or less than zero"
				};
			}
			var result = AccountManager.Data.FirstOrDefault(acc => acc.Type == accountType);
			if (result == null) {
				return new ApplicationResult {
					Success = false,
					Message = "Specified account type does not exist"
				};
			}
			if (CurrentAccount.Currency.Type != result.Currency.Type) {
				try {
					var converted = AccountCurrency.Convert(CurrentAccount.Currency, result.Currency, amount);
					CurrentAccount.Currency.Amount -= amount;
					result.Currency.Amount += converted;
					AccountManager.UpdateData();
				} catch (BadConversionException ex) {
					return new ApplicationResult {
						Success = false,
						Message = ex.Message
					};
				}
				return new ApplicationResult {
					Success = true,
					Message = $"Successfully converted and transfered to {result.Type}"
				};
			}
			CurrentAccount.Currency.Amount -= amount;
			result.Currency.Amount += amount;
			AccountManager.UpdateData();
			return new ApplicationResult {
				Success = true,
				Message = $"Successfully transfered to {result.Type}"
			};
		}
		private static void DoIncomeOrExpenseOperation(AccountActionType actionType, AccountCurrency targetCurrency, decimal finalAmount) {
			switch (actionType) {
				case AccountActionType.Expense: {
					targetCurrency.Substract(finalAmount);
					break;
				}
				case AccountActionType.Income: {
					targetCurrency.Add(finalAmount);
					break;
				}
			}
		}
		public static ApplicationResult AddIncomeOrExpense(AccountActionType actionType, string currencyType, decimal amount, string category, string note) {
			if (amount <= 0) {
				return new ApplicationResult {
					Success = false,
					Message = "Amount cannot be zero or less than zero"
				};
			}
			if (currencyType.ToLower() == "auto") {
				currencyType = CurrentAccount.Currency.Type;
			}
			var currentAccountCurrency = CurrentAccount.Currency;
			if (currencyType != CurrentAccount.Currency.Type) {
				try {
					var result = AccountCurrency.Convert(currencyType, CurrentAccount.Currency.Type, amount);
					DoIncomeOrExpenseOperation(actionType, currentAccountCurrency, result);
					AccountActionManager.AddAccountAction(new AccountAction(actionType, new AccountCurrency(CurrentAccount.Currency.Type, result), category, note));
					AccountActionManager.UpdateData();
					AccountManager.UpdateData();
				} catch (BadConversionException ex) {
					return new ApplicationResult {
						Success = false,
						Message = ex.Message
					};
				}
				return new ApplicationResult {
					Success = true,
					Message = "Successfully converted and added"
				};
			}
			DoIncomeOrExpenseOperation(actionType, currentAccountCurrency, amount);
			AccountActionManager.AddAccountAction(new AccountAction(actionType, new AccountCurrency(currencyType, amount), category, note));
			AccountActionManager.UpdateData();
			AccountManager.UpdateData();
			return new ApplicationResult {
				Success = true,
				Message = "Successfully added"
			};
		}
		public static ApplicationResult GetHistory(out AccountHistory history) {
			if (CurrentAccount == null) {
				history = null;
				return new ApplicationResult {
					Success = false,
					Message = "No active account"
				};
			}
			history = new AccountHistory(AccountActionManager.Data);
			return new ApplicationResult {
				Success = true,
				Message = string.Empty
			};
		}
		public static ApplicationResult GetAccountActions(out IReadOnlyList<AccountAction> accountActions) {
			if (CurrentAccount == null) {
				accountActions = null;
				return new ApplicationResult {
					Success = false,
					Message = "No active account"
				};
			}
			accountActions = AccountActionManager.Data;
			return new ApplicationResult {
				Success = true,
				Message = string.Empty
			};
		}
		public static ApplicationResult GetCurrentAccount(out Account acc) {
			if (CurrentAccount == null) {
				acc = null;
				return new ApplicationResult {
					Success = false,
					Message = "No active account"
				};
			}
			acc = CurrentAccount;
			return new ApplicationResult {
				Success = true,
				Message = string.Empty
			};
		}
		public static ApplicationResult RemoveAccount(string type) {
			var result = AccountManager.Data.FirstOrDefault(x => x.Type == type);
			if (result == null) {
				return new ApplicationResult {
					Success = false,
					Message = $"{type} does not exist"
				};
			}
			if (type == CurrentAccount.Type) {
				AccountManager.DeleteAccount(acc => acc == CurrentAccount);
				AccountManager.UpdateData();
				AccountActionManager.Destroy();
				AccountActionManager = null;
				if (AccountManager.Data.Count != 0) {
					CurrentAccount = AccountManager.Data.First();
				} else {
					CurrentAccount = null;
				}
			} else {
				new AccountActionManager(result.Id).Destroy();
				AccountManager.DeleteAccount(acc => acc.Type == type);
				AccountManager.UpdateData();
			}
			return new ApplicationResult {
				Success = true,
				Message = "Successfully deleted"
			};
		}
	}
}
