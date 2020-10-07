using System;

namespace MonefyConsole {
	static class DateTimeExtensions {
		public static DateTime GetBeginOfDay(this DateTime source) {
			return new DateTime(source.Year, source.Month, source.Day);
		}
		public static DateTime GetBeginOfWeek(this DateTime source) {
			int daysToSubstract;
			if (source.DayOfWeek == DayOfWeek.Sunday) {
				daysToSubstract = 6;
			} else {
				daysToSubstract = ((int)source.DayOfWeek) - 1;
			}
			return source.Subtract(new TimeSpan(daysToSubstract, source.Hour, source.Minute, source.Second));
		}
		public static DateTime GetBeginOfMonth(this DateTime source) {
			return source.Subtract(new TimeSpan(source.Day - 1, source.Hour, source.Minute, source.Second));
		}
		public static DateTime GetBeginOfYear(this DateTime source) {
			return source.Subtract(new TimeSpan(source.DayOfYear - 1, source.Hour, source.Minute, source.Second));
		}
	}
}
