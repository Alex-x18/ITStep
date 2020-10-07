using System;
using System.Collections.Generic;
using System.IO;
using System.Text.Json;

namespace MonefyConsole {
	class AccountActionManager : IMemoralbe<AccountAction>, IDisposable {
		~AccountActionManager() {
			Dispose();
		}
		public AccountActionManager(Guid relatedAccountGuid) {
			Filename = $"History_{relatedAccountGuid}.json";
			Data = new List<AccountAction>();
			fileStream = new FileStream(Filename, FileMode.OpenOrCreate, FileAccess.ReadWrite);
			streamWriter = new StreamWriter(stream: fileStream, leaveOpen: true);
			streamReader = new StreamReader(stream: fileStream, leaveOpen: true);
		}
		public string Filename { get; }
		public IReadOnlyList<AccountAction> Data { get; }
		public void AddAccountAction(AccountAction accountAction) {
			if (Data is List<AccountAction> dataList) {
				dataList.Add(accountAction);
			}
		}
		public void EditAccountAction(AccountAction accountAction, AccountCurrency currency = null, string category = null, string note = null) {
			if (currency == null && category == null && note == null) {
				return;
			}
			if (currency != null) {
				accountAction.Currency.Amount = currency.Amount;
				accountAction.Currency.Type = currency.Type;
			}
			if (category != null) {
				accountAction.Category = category;
			}
			if (note != null) {
				accountAction.Note = note;
			}
		}
		public void Destroy() {
			ReleaseData();
			Dispose();
			File.Delete(Filename);
		}
		public void LoadData() {
			try {
				if (Data is List<AccountAction> dataList) {
					dataList.Clear();
					var data = streamReader.ReadToEnd();
					var result = JsonSerializer.Deserialize<List<AccountAction>>(data);
					dataList.AddRange(result);
				}
			} catch { }
		}
		public void ReleaseData() {
			if (Data is List<AccountAction> dataList) {
				dataList.Clear();
			}
			try {
				fileStream.Seek(0, SeekOrigin.End);
				fileStream.SetLength(0);
			} catch { }
		}
		public void UpdateData() {
			fileStream.Seek(0, SeekOrigin.Begin);
			fileStream.SetLength(0);
			streamWriter.Write(JsonSerializer.Serialize(Data as List<AccountAction>));
			streamWriter.Flush();
		}
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
		private readonly FileStream fileStream;
		private readonly StreamReader streamReader;
		private readonly StreamWriter streamWriter;
	}
}
