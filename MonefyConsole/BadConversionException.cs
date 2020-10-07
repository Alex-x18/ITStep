using System;

namespace MonefyConsole {
	class BadConversionException : Exception {
		public BadConversionException(string message) : base(message) { }
	}
}