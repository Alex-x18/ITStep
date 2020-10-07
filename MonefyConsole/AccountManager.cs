using System;
using System.Collections.Generic;
using System.IO;
using System.Text.Json;

namespace MonefyConsole {
	class AccountManager : IMemoralbe<Account>, IDisposable {
		~AccountManager() {
			Dispose();
		}
		public AccountManager(string fileName) {
			Filename = fileName;
			Data = new List<Account>();
			fileStream = new FileStream(Filename, FileMode.OpenOrCreate, FileAccess.ReadWrite);
			streamReader = new StreamReader(stream: fileStream, leaveOpen: true);
			streamWriter = new StreamWriter(stream: fileStream, leaveOpen: true);
		}
		public string Filename { get; }
		public IReadOnlyList<Account> Data { get; }
		public event EventHandler AccountAdded;
		public void AddAccount(AccountCurrency currency, string type, string name) {
			if (Data is List<Account> dataList) {
				var acc = new Account(currency, type, name);
				dataList.Add(acc);
				AccountAdded?.Invoke(this, new AccountManagerEventArgs(acc, AccountManagerEventType.AccountAdded));
			}
		}
		public void AddAccount(string currencyType, string type, string name) {
			AddAccount(currencyType, type, name);
		}
		public event EventHandler AccountDeleted;
		public void DeleteAccount(Predicate<Account> predicate) { 
			if (Data is List<Account> dataList) {
				foreach (var item in dataList) {
					if (predicate(item)) {
						dataList.Remove(item);
						AccountDeleted?.Invoke(this, new AccountManagerEventArgs(item, AccountManagerEventType.AccountDeleted));
						return;
					}
				}
			}
		}
		public void LoadData() {
			try {
				if (Data is List<Account> dataList) {
					dataList.Clear();
					var data = streamReader.ReadToEnd();
					var result = JsonSerializer.Deserialize<List<Account>>(data);
					dataList.AddRange(result);
				}
			} catch { }
 		}
		public void ReleaseData() {
			(Data as List<Account>).Clear();
			try {
				fileStream.Seek(0, SeekOrigin.End);
				fileStream.SetLength(0);
			} catch { }
		}
		public event EventHandler AccountUpdated;
		public void UpdateAccount(Account account, string currency = null, string type = null, string name = null) { 
			if (currency == null && type == null && name == null) {
				return;
			}
			var result = Data.Find(acc => acc == account);
			if (result != null) {
				if (currency != null) {
					result.Currency.Type = currency;
				}
				if (type != null) {
					result.Type = type;
				}
				if (name != null) {
					result.Name = name;
				}
				AccountUpdated?.Invoke(this, new AccountManagerEventArgs(account, AccountManagerEventType.AccountUpdated));
			}
		}
		public void UpdateData() {
			fileStream.Seek(0, SeekOrigin.Begin);
			fileStream.SetLength(0);
			streamWriter.Write(JsonSerializer.Serialize(Data as List<Account>));
			streamWriter.Flush();
		}
		private readonly FileStream fileStream;
		private readonly StreamReader streamReader;
		private readonly StreamWriter streamWriter;
		private bool disposed = false;
		public void Dispose() {
			if (!disposed) {
				streamReader.Dispose();
				streamWriter.Dispose();
				fileStream.Dispose();
				disposed = true;
				GC.SuppressFinalize(this);
			}
		}
	}
}
