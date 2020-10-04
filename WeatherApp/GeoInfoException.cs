using System;

namespace WeatherApp {
	class GeoInfoException : Exception {
		public GeoInfoException(string message): base(message) { }
	}
}
