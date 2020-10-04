using System;
using System.Text.Json;
using System.Net;
using System.Net.Sockets;
using System.IO;
using System.Threading;
using System.Threading.Tasks;

namespace WeatherApp {
	class Program {
		static async Task Main(string[] args) {
			if (!File.Exists("GeoApiKey")) {
				Console.WriteLine($"Couldn't find GeoApiKey file in {Directory.GetCurrentDirectory()}");
				return;
			}
			if (!File.Exists("WeatherApiKey")) {
				Console.WriteLine($"Couldn't find WeatherApiKey file in {Directory.GetCurrentDirectory()}");
				return;
			}
			var geo = new GeoInfo(File.ReadAllText("GeoApiKey"));
			var weather = new WeatherWidget(File.ReadAllText("WeatherApiKey"), geo);
			var updaterCts = new CancellationTokenSource();
			var updater = new Task(async () => {
				do {
					try {
						geo.Update();
						weather.Update();
					} catch(GeoInfoException ex) {
						Console.WriteLine($"> {ex.Message}");
					} catch (Exception) {
						Console.WriteLine("> Unexpected error");
						return;
					}
					Console.Write(weather.ToString());
					try {
						await Task.Delay(1000 * 60 * 60, updaterCts.Token);
						Console.Clear();
					} catch(OperationCanceledException) {
						return;
					}
				} while (true);
			});
			var consoleCancelAwaiter = new Task(() => {
				do {
					var conKey = Console.ReadKey(true);
					if (conKey.Key == ConsoleKey.Escape) {
						updaterCts.Cancel();
						return;
					}
				} while (true);
			});
			consoleCancelAwaiter.Start();
			updater.Start();
			try {
				await consoleCancelAwaiter;
			} finally {
				consoleCancelAwaiter.Dispose();
				updater.Dispose();
			}
		}
	}
}