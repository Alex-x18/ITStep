using System;

namespace MonefyConsole {
	class AccountAction {
		public AccountAction() { }
		public AccountAction(AccountActionType type, AccountCurrency currency, string category, string note) {
			Timestamp = DateTime.Now;
			Type = type;
			Note = note;
			Category = category;
			Currency = currency;
		}
		public DateTime Timestamp { get; set; }
		public AccountActionType Type { get; set; }
		public string Note { get; set; }
		public string Category { get; set; }
		public AccountCurrency Currency { get; set; }
	}
}
