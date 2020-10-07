using System;
using System.Collections.Generic;
using System.Linq;

namespace MonefyConsole {
	static class ConsoleOutput {
		public static void ShowApplicationResult(ApplicationResult result) {
			Console.WriteLine($"{(result.Success ? '>' : '!')} {result.Message}");
		}
		public static void ShowAccountInfo() {
			var result = Application.GetCurrentAccountInfo(out string info);
			if (result.Success) {
				Console.WriteLine($"> {info}");
			} else {
				ShowApplicationResult(result);
			}
		}
		public static void ShowRecordsTypeMenu() {
			Console.Clear();
			Console.WriteLine("1. All Records");
			Console.WriteLine("2. Records by Categories");
			Console.WriteLine("X. Back");
		}
		public static void ShowMainMenu(bool condition) {
			if (condition) {
				Console.WriteLine("1. Income");
				Console.WriteLine("2. Expense");
				Console.WriteLine("3. Records");
				Console.WriteLine("4. Transfer to Account");
				Console.WriteLine("5. Account Info");
				Console.WriteLine("6. Account Settings");				
				Console.WriteLine("X. Exit");
			} else {
				Console.WriteLine("1. Create Account");
				Console.WriteLine("2. Switch Account");
				Console.WriteLine("X. Exit");
			}
		}
		public static void ShowAccountsSetingsMenu() {
			Console.Clear();
			Console.WriteLine("1. Switch Account");
			Console.WriteLine("2. Modify Account");
			Console.WriteLine("3. Create Account");
			Console.WriteLine("4. Remove Account");
			Console.WriteLine("X. Back");
		}
		public static void ShowRecordsMenu() {
			Console.Clear();
			Console.WriteLine("1. Daily Records");
			Console.WriteLine("2. Weekly Records");
			Console.WriteLine("3. Monthly Records");
			Console.WriteLine("4. Yearly Records");
			Console.WriteLine("5. Show all Records");
		}
		public static void ShowAllRecords() {
			Application.GetAccountActions(out IReadOnlyList<AccountAction> actions);
			if (actions.Count == 0) {
				ShowRecords(null);
				return;
			}
			var firstAction = actions[0];
			var record = new AccountRecord(actions, new AccountCurrency(firstAction.Currency.Type, actions.Sum(act => act.Currency.Amount)), firstAction.Timestamp, DateTime.Now);
			ShowRecords(record);
		}
		public static void ShowAllRecordsByGroup() {
			Application.GetAccountActions(out IReadOnlyList<AccountAction> actions);
			if (actions.Count == 0) {
				ShowRecordsByCategories(null);
				return;
			}
			var firstAction = actions[0];
			var record = new AccountRecord(actions, new AccountCurrency(firstAction.Currency.Type, actions.Sum(act => act.Currency.Amount)), firstAction.Timestamp, DateTime.Now);
			ShowRecordsByCategories(record);
		}
		public static void ShowWeeklyRecords() {
			var res = Application.GetAccountActions(out IReadOnlyList<AccountAction> actions);
			if (res.Success) {
				ShowRecords(new AccountHistory(actions).FetchWeeklyRecord());
			} else {
				ShowApplicationResult(res);
			}
		}
		public static void ShowWeeklyRecordsByGroup() {
			var res = Application.GetAccountActions(out IReadOnlyList<AccountAction> actions);
			if (res.Success) {
				ShowRecordsByCategories(new AccountHistory(actions).FetchWeeklyRecord());
			} else {
				ShowApplicationResult(res);
			}
		}
		public static void ShowMonthlyRecords() {
			var res = Application.GetAccountActions(out IReadOnlyList<AccountAction> actions);
			if (res.Success) {
				ShowRecords(new AccountHistory(actions).FetchMonthlyRecord());
			} else {
				ShowApplicationResult(res);
			}
		}
		public static void ShowMonthlyRecordsByGroup() {
			var res = Application.GetAccountActions(out IReadOnlyList<AccountAction> actions);
			if (res.Success) {
				ShowRecordsByCategories(new AccountHistory(actions).FetchMonthlyRecord());
			} else {
				ShowApplicationResult(res);
			}
		}
		public static void ShowDailyRecords() {
			var res = Application.GetAccountActions(out IReadOnlyList<AccountAction> actions);
			if (res.Success) {
				ShowRecords(new AccountHistory(actions).FetchDailyRecord());
			} else {
				ShowApplicationResult(res);
			}
		}
		public static void ShowDailyRecordsByGroup() {
			var res = Application.GetAccountActions(out IReadOnlyList<AccountAction> actions);
			if (res.Success) {
				ShowRecordsByCategories(new AccountHistory(actions).FetchDailyRecord());
			} else {
				ShowApplicationResult(res);
			}
		}
		public static void ShowYearlyRecords() {
			var res = Application.GetAccountActions(out IReadOnlyList<AccountAction> actions);
			if (res.Success) {
				ShowRecords(new AccountHistory(actions).FetchYearlyRecord());
			} else {
				ShowApplicationResult(res);
			}
		}
		public static void ShowYearlyRecordsByGroup() {
			var res = Application.GetAccountActions(out IReadOnlyList<AccountAction> actions);
			if (res.Success) {
				ShowRecordsByCategories(new AccountHistory(actions).FetchYearlyRecord());
			} else {
				ShowApplicationResult(res);
			}
		}
		public static void ShowRecords(AccountRecord record) {
			if (record == null) {
				ShowApplicationResult(new ApplicationResult {
					Success = false,
					Message = "No records"
				});
				return;
			}
			int counter = 1;
			Console.WriteLine();
			Console.WriteLine($"> [{record.StartDate} <=> {record.EndDate}: {record.Result}]");
			foreach(var item in record.Actions) {
				Console.WriteLine($"> {counter++}:");
				Console.WriteLine($"\t> {item.Type}: {item.Currency}");
				Console.WriteLine($"\t> Category: {item.Category}");
				Console.WriteLine($"\t> Note: {item.Note}");
				Console.WriteLine($"\t> Date: {item.Timestamp}");
			}
		}
		public static void ShowRecordsByCategories(AccountRecord record) {
			if (record == null || record.Actions.Count == 0) {
				ShowApplicationResult(new ApplicationResult {
					Success = false,
					Message = "No records"
				});
				return;
			}
			var expenseActions = record.Actions.Where(action => action.Type == AccountActionType.Expense);
			var totalExpensedSum = expenseActions.Sum(action => action.Currency.Amount);
			var cats = expenseActions.GroupBy(action => action.Category);
			var currencyType = record.Actions[0].Currency.Type;
			var counter = 1;
			Console.WriteLine();
			Console.WriteLine($"> [{record.StartDate} <=> {record.EndDate}: {record.Result}]");
			foreach (var g in cats) {
				var sum = g.Sum(action => action.Currency.Amount);
				Console.WriteLine($"> {counter++}:");
				Console.WriteLine($"\t> {currencyType} {sum} : {Math.Round(sum / totalExpensedSum * 100, 1)}%");
				Console.WriteLine($"\t> Category: {g.Key}");
			}
		}
	}
}