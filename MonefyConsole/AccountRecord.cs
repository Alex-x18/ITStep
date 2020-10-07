using System;
using System.Collections.Generic;

namespace MonefyConsole {
	class AccountRecord {
		public AccountRecord(IReadOnlyList<AccountAction> actions, AccountCurrency result, DateTime startDate, DateTime endDate) {
			Result = result;
			StartDate = startDate;
			EndDate = endDate;
			Actions = actions;
		}
		public IReadOnlyList<AccountAction> Actions { get; }
		public AccountCurrency Result { get; }
		public DateTime StartDate { get; }
		public DateTime EndDate { get; }
	}
}
