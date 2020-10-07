using System;

namespace MonefyConsole {
	class Account  {
		public Account() { }
		public Account(AccountCurrency currency, string type, string name) {
			Name = name;
			Currency = currency;
			Type = type;
			AccountCreatedTimestamp = DateTime.Now;
			Id = Guid.NewGuid();
		}
		public string Name { get; set; }
		public AccountCurrency Currency { get; set; }
		public string Type { get; set; }
		public DateTime AccountCreatedTimestamp { get; set; }
		public Guid Id { get; set; }
	}
}
