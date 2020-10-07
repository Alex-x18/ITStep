using System;
using System.IO;

namespace MonefyConsole {
	class AccountCurrency {
		public AccountCurrency() { }
		public AccountCurrency(string type, decimal amount = 0) {
			Type = type.ToUpper();
			Amount = amount;
		}
		public string Type { get; set; }
		public decimal Amount { get; set; }
		public void Add(decimal value) {
			Amount += value;
		}
		public void Substract(decimal value) {
			Amount -= value;
		}
		public static bool operator >(AccountCurrency left, decimal right) => left.Amount > right;
		public static bool operator <(AccountCurrency left, decimal right) => left.Amount < right;
		public static implicit operator AccountCurrency(string currency) => new AccountCurrency(currency);
		public const string FilePath = "Currencies.csv";
		public static decimal Convert(AccountCurrency sourceCurrency, AccountCurrency targetCurrency, decimal? amount = null) {
			if (amount == null) {
				amount = targetCurrency.Amount;
			}
			if (targetCurrency.Type == sourceCurrency.Type) {
				return targetCurrency.Amount;
			}
			string[] lines = File.ReadAllLines(FilePath);
			int? convertIndexCol = null;
			int? convertIndexRow = null;
			decimal mod;
			try {
				var types = lines[0].Split(',');
				for (int i = 1; i < types.Length; i++) {
					if (types[i] == sourceCurrency.Type) {
						convertIndexRow = i;
						break;
					}
				}
				if (convertIndexRow == null) {
					throw new Exception($"{sourceCurrency.Type} -> {targetCurrency.Type}");
				}
				for (int i = 1; i < lines.Length; i++) {
					var data = lines[i].Split(',');
					if (data[0] == targetCurrency.Type) {
						convertIndexCol = i;
					}
				}
				if (convertIndexCol == null) {
					throw new Exception($"{sourceCurrency.Type} -> {targetCurrency.Type}");
				}
				mod = decimal.Parse(lines[(int)convertIndexCol].Split(',')[(int)convertIndexRow]);
			} catch (Exception ex) {
				throw new BadConversionException($"Bad converison: {ex.Message}");
			}
			return (decimal)amount * mod;
		}
		public static bool Exists(string type) {
			type = type.ToUpper();
			var lines = File.ReadAllLines(FilePath);
			try {
				var types = lines[0].Split(',');
				for (int i = 1; i < types.Length; i++) {
					if (types[i] == type) {
						return true;
					}
				}
				return false;
			} catch {
				return false;
			}
		}
		public override string ToString() {
			return $"{Amount} {Type}";
		}
	}
}
