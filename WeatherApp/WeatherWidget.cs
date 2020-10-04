using System;
using System.Collections.Generic;
using System.Text;
using System.Net;
using System.Text.Json;

namespace WeatherApp {
	public class WeatherWidget {
		public WeatherWidget(string weatherApiKey, GeoInfo info) {
			GInfo = info;
			Response = null;
			ApiKey = weatherApiKey;
			if (ApiKey == null || info == null) {
				throw new ArgumentNullException("Malformed object: an argument cannot be null");
			}
		}
		public void Update() {
			if (GInfo.Lat == null || GInfo.Lon == null) {
				throw new GeoInfoException("Latitude or Longitude cannot be null");
			}
			using var client = new WebClient();
			var response = client.DownloadString($"http://api.openweathermap.org/data/2.5/weather?lat={GInfo.Lat}&lon={GInfo.Lon}&appid={ApiKey}");
			Response = JsonSerializer.Deserialize<WeatherWidgetResponse>(response);
		}
		public double? TempKelvin {
			get {
				return Response?.main.temp;
			}
		}
		public double? TempCelsius {
			get {
				return Response?.main?.temp - 273.15;
			}
		}
		public double? TempFahrenheit {
			get {
				return (Response?.main?.temp - 273.15) * 9 / 5 + 32;
			}
		}
		public int? Humidity {
			get {
				return Response?.main?.humidity;
			}
		}
		public int? Pressure {
			get {
				return Response?.main?.pressure;
			}
		}
		public List<string> Description {
			get {
				var list = new List<string>();
				foreach (var item in Response?.weather) {
					list.Add(item.main);
				}
				return list;
			}
		}
		private WeatherWidgetResponse Response { get; set; }
		private GeoInfo GInfo { get; }
		private string ApiKey { get; }
		public override string ToString() {
			var builder = new StringBuilder();
			if (GInfo.City != null) {
				builder.Append($"@ City: {GInfo.City}\n");
			}
			if (GInfo.Country != null) {
				builder.Append($"@ Country: {GInfo.Country}\n");
			}
			if (TempKelvin != null) {
				builder.Append($"@ Kelvin: {TempKelvin}K\n");
			}
			if (TempFahrenheit != null) {
				builder.Append($"@ Fahrenheit: {TempFahrenheit}F\n");
			}
			if (TempCelsius != null) {
				builder.Append($"@ Celsius: {TempCelsius}°C\n");
			}
			if (Humidity != null) {
				builder.Append($"@ Humidity: {Humidity}%\n");
			}
			if (Pressure != null) {
				builder.Append($"@ Pressure: {Pressure}Pa\n");
			}
			if (Description.Count != 0) {
				builder.Append($"@ Description: ");
				for (int i = 0; i < Description.Count; i++) {
					builder.Append(Description[i]);
					if (i != Description.Count - 1) {
						builder.Append("; ");
					}
				}
				builder.Append('\n');
			}
			return builder.ToString();
		}
	}
}
