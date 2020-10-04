using System;
using System.Collections.Generic;
using System.Net;
using System.Text;
using System.Text.Json;
using System.IO;

namespace TranslateApp {
	class Program {
		static void Main() {
			if (!File.Exists("ApiKey")) {
				Console.WriteLine($"Couldn't find ApiKey file in {Directory.GetCurrentDirectory()}");
				return;
			}
			Console.OutputEncoding = Encoding.UTF8;
			using var googleTranslator = new ApiedGoogleTranslator(File.ReadAllText("ApiKey"));
			try {
				var availableLanguages = googleTranslator.GetLanguages();
				string sourceLanguage;
				string targetLanguage;
				string text;
				do {
					Console.Write("> Available languages: ");
					foreach (var item in availableLanguages) {
						Console.Write($"{item} ");
					}
					Console.Write('\n');
					Console.Write("@ Specify source language (use auto for autodetection): ");
					sourceLanguage = Console.ReadLine();
					if (!availableLanguages.Contains(sourceLanguage) && sourceLanguage != "auto") {
						sourceLanguage = "auto";
						Console.WriteLine("> Misleading source language; auto is used");
					}
					Console.Write("@ Write text to translate: ");
					text = Console.ReadLine();
					do {
						Console.Write("@ Specify target language: ");
						targetLanguage = Console.ReadLine();
						if (!availableLanguages.Contains(targetLanguage)) {
							Console.WriteLine("> Invalid target language");
							continue;
						}
						if (sourceLanguage == targetLanguage) {
							Console.WriteLine("> Source and target languages are the same");
							continue;
						}
						break;
					} while (true);
					var translations = sourceLanguage == "auto" ? googleTranslator.Translate(targetLanguage, text) : googleTranslator.Translate(sourceLanguage, targetLanguage, text);
					if (translations.Count == 0) {
						Console.WriteLine("> Failed to translate");
					} else {
						foreach (var item in translations) {
							Console.WriteLine($"> {item}");
						}
					}
					Console.WriteLine("Press Esc to exit or any key to continue...");
					var key = Console.ReadKey();
					if (key.Key == ConsoleKey.Escape) {
						break;
					}
					Console.Clear();
				} while (true);
			} catch (WebException ex) {
				Console.WriteLine($"{ex.Status}: {ex.Message}");
			}
		}
	}
}