using System;
using System.Collections.Generic;
using System.IO;

namespace MonefyConsole {
	static class ConsoleInput {
		private static bool SafeDecimalInput(out decimal amount) {
			try {
				amount = decimal.Parse(Console.ReadLine());
				return true;
			} catch {
				amount = default;
				return false;
			}
		}
		private static bool suspendAfterMainAction = true;
		public static void Begin() {
			if (!File.Exists(AccountCurrency.FilePath)) {
				Console.WriteLine($"{AccountCurrency.FilePath} is required; Fallback file is created");
				var fallback_content = $"_,AZN,EUR,USD\n"
					+ "AZN,1,1,1\n"
					+ "EUR,1,1,1\n"
					+ "USD,1,1,1\n";
				using FileStream fs = new FileStream(AccountCurrency.FilePath, FileMode.Create, FileAccess.Write);
				using StreamWriter sw = new StreamWriter(fs);
				sw.Write(fallback_content);
			}
			do {
				Console.WriteLine("# Money Manager v0.1b");
				var accountActive = Application.IsAccountActive();
				ConsoleOutput.ShowMainMenu(accountActive);
				suspendAfterMainAction = true;
				if (accountActive) {
					if (!AccountActiveSwitch()) {
						break;
					}
				} else {
					if (!AccountInactiveSwitch()) {
						break;
					}
				}
				if (suspendAfterMainAction) {
					Console.ReadKey(true);
				}
				suspendAfterMainAction = true;
				Console.Clear();
			} while (true);
		}
		public static bool AccountInactiveSwitch() {
			var key = Console.ReadKey(true).Key;
			switch (key) {
				case ConsoleKey.D1: {
					CreateAccount();
					break;
				}
				case ConsoleKey.D2: {
					SwitchAccount();
					break;
				}
				case ConsoleKey.X: {
					return false;
				}
			}
			return true;
		}
		public static bool AccountActiveSwitch() {
			var key = Console.ReadKey(true).Key;
			switch (key) {
				case ConsoleKey.D1: {
					IncomeOrExpense(AccountActionType.Income);
					break;
				}
				case ConsoleKey.D2: {
					IncomeOrExpense(AccountActionType.Expense);
					break;
				}
				case ConsoleKey.D3: {
					ConsoleOutput.ShowRecordsTypeMenu();
					var keyRecordsType = Console.ReadKey(true).Key;
					switch (keyRecordsType) {
						case ConsoleKey.D1: {
							ConsoleOutput.ShowRecordsMenu();
							RecordSwitch();
							break;
						}
						case ConsoleKey.D2: {
							ConsoleOutput.ShowRecordsMenu();
							RecordByCategorySwitch();
							break;
						}
						case ConsoleKey.X: {
							suspendAfterMainAction = false;
							break;
						}
					}
					break;
				}
				case ConsoleKey.D4: {
					TranferToAccount();
					break;
				}
				case ConsoleKey.D5: {
					ConsoleOutput.ShowAccountInfo();
					break;
				}
				case ConsoleKey.D6: {
					ConsoleOutput.ShowAccountsSetingsMenu();
					AccountSettingsSwitch();
					break;
				}
				case ConsoleKey.X: {
					suspendAfterMainAction = false;
					return false;
				}
			}
			return true;
		}
		public static void RemoveAccount() {
			var result = Application.GetAllAccountTypes(out IReadOnlyList<string> types);
			if (!result.Success) {
				ConsoleOutput.ShowApplicationResult(result);
				return;
			}
			Console.Write("> Avaliable accounts: ");
			for (int i = 0; i < types.Count; i++) {
				Console.Write(types[i]);
				if (i != types.Count - 1) {
					Console.Write(", ");
				}
			}
			Console.WriteLine();
			Console.Write("@ Specify account type > ");
			var targetType = Console.ReadLine();
			var res = Application.RemoveAccount(targetType);
			ConsoleOutput.ShowApplicationResult(res);
		}
		public static void AccountSettingsSwitch() {
			var key = Console.ReadKey(true).Key;
			switch (key) {
				case ConsoleKey.D1: {
					SwitchAccount();
					break;
				}
				case ConsoleKey.D2: {
					ModifyAccount();
					break;
				}
				case ConsoleKey.D3: {
					CreateAccount();
					break;
				}
				case ConsoleKey.D4: {
					RemoveAccount();
					break;
				}
				case ConsoleKey.X: {
					suspendAfterMainAction = false;
					return;
				}
			}
		}
		public static void RecordByCategorySwitch() {
			var recordKeySwitch = Console.ReadKey(true).Key;
			switch (recordKeySwitch) {
				case ConsoleKey.D1: {
					ConsoleOutput.ShowDailyRecordsByGroup();
					break;
				}
				case ConsoleKey.D2: {
					ConsoleOutput.ShowWeeklyRecordsByGroup();
					break;
				}
				case ConsoleKey.D3: {
					ConsoleOutput.ShowMonthlyRecordsByGroup();
					break;
				}
				case ConsoleKey.D4: {
					ConsoleOutput.ShowYearlyRecordsByGroup();
					break;
				}
				case ConsoleKey.D5: {
					ConsoleOutput.ShowAllRecordsByGroup();
					break;
				}
			}
		}
		public static void RecordSwitch() {
			var recordKeySwitch = Console.ReadKey(true).Key;
			switch (recordKeySwitch) {
				case ConsoleKey.D1: {
					ConsoleOutput.ShowDailyRecords();
					break;
				}
				case ConsoleKey.D2: {
					ConsoleOutput.ShowWeeklyRecords();
					break;
				}
				case ConsoleKey.D3: {
					ConsoleOutput.ShowMonthlyRecords();
					break;
				}
				case ConsoleKey.D4: {
					ConsoleOutput.ShowYearlyRecords();
					break;
				}
				case ConsoleKey.D5: {
					ConsoleOutput.ShowAllRecords();
					break;
				}
			}
		}
		public static void CreateAccount() {
			Console.CursorVisible = true;
			string alias;
			string currencyType;
			string accountType;
			Console.Write("@ Enter your name/nickname > ");
			alias = Console.ReadLine();
			Console.Write("@ Enter currency > ");
			currencyType = Console.ReadLine().ToUpper();
			Console.Write("@ Enter account type > ");
			accountType = Console.ReadLine();
			Console.Write("@ Enter initial amount > ");
			if (!SafeDecimalInput(out decimal amount)) {
				ConsoleOutput.ShowApplicationResult(new ApplicationResult {
					Success = false,
					Message = "Invalid input"
				});
				return;
			}
			ConsoleOutput.ShowApplicationResult(Application.CreateAccount(alias, currencyType, accountType, amount));
			Console.CursorVisible = false;
		}
		public static void SwitchAccount() {                                      
			Console.CursorVisible = true;
			var result = Application.GetAllAccountTypes(out IReadOnlyList<string> accountTypes);
			if (result.Success) {
				Console.Write("> Avaliable accounts: ");
				for (int i = 0; i < accountTypes.Count; i++) {
					Console.Write(accountTypes[i]);
					if (i != accountTypes.Count - 1) {
						Console.Write(", ");
					}
				}
				Console.WriteLine();
				Console.Write("@ Specify account type > ");
				string newAccountType = Console.ReadLine();
				ConsoleOutput.ShowApplicationResult(Application.SwitchAccount(newAccountType));
			} else {
				ConsoleOutput.ShowApplicationResult(result);
			}
			Console.CursorVisible = false;
		}
		public static void IncomeOrExpense(AccountActionType accountActionType) {
			Console.CursorVisible = true;
			string currencyType;
			string category;
			string note;
			Console.Write("@ Specify currency type > ");
			currencyType = Console.ReadLine().ToUpper();
			Console.Write("@ Specify amount > ");
			if (!SafeDecimalInput(out decimal amount)) {
				ConsoleOutput.ShowApplicationResult(new ApplicationResult {
					Success = false,
					Message = "Invalid input"
				});
				return;
			}
			Console.Write("@ Specify category > ");
			category = Console.ReadLine();
			Console.Write("@ Specify note > ");
			note = Console.ReadLine();
			ConsoleOutput.ShowApplicationResult(Application.AddIncomeOrExpense(accountActionType, currencyType, amount, category, note));
			Console.CursorVisible = false;
		}
		public static void ModifyAccount() {
			Console.CursorVisible = true;
			string newCurrencyType;
			string newAccountType;
			Console.Write("@ Currency type > ");
			newCurrencyType = Console.ReadLine().ToUpper();
			if (string.IsNullOrEmpty(newCurrencyType)) {
				newCurrencyType = null;
			}
			Console.Write("@ Account type > ");
			newAccountType = Console.ReadLine();
			if (string.IsNullOrEmpty(newAccountType)) {
				newAccountType = null;
			}
			ConsoleOutput.ShowApplicationResult(Application.ModifyAccount(newCurrencyType, newAccountType));
			Console.CursorVisible = false;
		}
		public static void TranferToAccount() {
			Console.CursorVisible = true;
			string accountType;
			Console.Write("@ Specify account type > ");
			accountType = Console.ReadLine();
			Console.Write("@ Specify amount > ");
			if (!SafeDecimalInput(out decimal amount)) {
				ConsoleOutput.ShowApplicationResult(new ApplicationResult {
					Success = false,
					Message = "Invalid input"
				});
				return;
			}
			ConsoleOutput.ShowApplicationResult(Application.TransferToAccount(accountType, amount));
			Console.CursorVisible = !true;
		}
	}
}