using System;
using System.Collections.Generic;

namespace MonefyConsole {
	static class IReadOnlyListExtensions {
		public static T Find<T>(this IReadOnlyList<T> source, Predicate<T> pred) {
			foreach (var item in source) {
				if (pred(item)) {
					return item;
				}
			}
			return default;
		}
		public static bool Exists<T>(this IReadOnlyList<T> source, Predicate<T> pred) {
			foreach (var item in source) {
				if (pred(item)) {
					return true;
				}
			}
			return false;
		}
	}
}
