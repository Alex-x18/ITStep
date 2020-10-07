using System;
using System.Collections.Generic;
using System.Text;

namespace MonefyConsole {
	class AccountHistory {
		public AccountHistory(IReadOnlyList<AccountAction> actions) {
			this.actions = actions;
		}
		public AccountRecord FetchRecord(DateTime start, DateTime end) {
			if (actions.Count == 0) {
				return null;
			}
			var currency = new AccountCurrency(actions[0].Currency.Type);
			var includedActions = new List<AccountAction>();
			var conclusion = new AccountRecord(includedActions, currency, start, end);
			foreach (var item in actions) {
				if (item.Timestamp >= start && item.Timestamp <= end) {
					switch (item.Type) {
						case AccountActionType.Expense: {
							conclusion.Result.Amount -= item.Currency.Amount;
							break;
						}
						case AccountActionType.Income: {
							conclusion.Result.Amount += item.Currency.Amount;
							break;
						}
					}
					includedActions.Add(item);
				}
			}
			return conclusion;
		}
		public AccountRecord FetchDailyRecord() => FetchRecord(DateTime.Now.GetBeginOfDay(), DateTime.Now);
		public AccountRecord FetchWeeklyRecord() => FetchRecord(DateTime.Now.GetBeginOfWeek(), DateTime.Now);
		public AccountRecord FetchMonthlyRecord() => FetchRecord(DateTime.Now.GetBeginOfMonth(), DateTime.Now);
		public AccountRecord FetchYearlyRecord() => FetchRecord(DateTime.Now.GetBeginOfYear(), DateTime.Now);
		private readonly IReadOnlyList<AccountAction> actions;
	}
}
 