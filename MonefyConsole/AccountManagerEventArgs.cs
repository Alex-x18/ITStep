using System;

namespace MonefyConsole {
	class AccountManagerEventArgs : EventArgs {
		public AccountManagerEventArgs(Account target, AccountManagerEventType eventType) {
			Target = target;
			EventType = eventType;
		}
		public Account Target;
		public AccountManagerEventType EventType;
	}
}
