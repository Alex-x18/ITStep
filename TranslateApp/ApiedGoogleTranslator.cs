using System;
using System.Collections.Generic;
using System.Text.Json;
using System.Net;
using System.IO;

namespace TranslateApp {
	class ApiedGoogleTranslator : IDisposable {
		public ApiedGoogleTranslator(string apiKey) {
			ApiKey = apiKey;
			WClient = new WebClient();
		}
		public List<string> GetLanguages() {
			var response = WClient.DownloadString($"https://translation.googleapis.com/language/translate/v2/languages?key={ApiKey}");
			var deserialized = JsonSerializer.Deserialize<SupportedLanguagesResponse>(response).data.languages;
			var result = new List<string>();
			foreach (var item in deserialized) {
				result.Add(item.language);
			}
			return result;
		}
		public string DetectLanguage(string text) {
			var response = WClient.UploadString($"https://translation.googleapis.com/language/translate/v2/detect?key={ApiKey}", JsonSerializer.Serialize(new DetectLanguageRequest() { q = text }));
			var deserialized = JsonSerializer.Deserialize<DetectLanguageResponse>(response);
			double maxConfidence = 0;
			string targetLang = null;
			foreach (var detection in deserialized.data.detections) {
				foreach (var lang in detection) {
					if (lang.confidence > maxConfidence) {
						maxConfidence = lang.confidence;
						targetLang = lang.language;
					}
				}
			}
			return targetLang;
		}
		public List<string> Translate(string sourceLang, string targetLang, string text) {
			var response = WClient.UploadString(@$"https://translation.googleapis.com/language/translate/v2?key={ApiKey}", JsonSerializer.Serialize(new TranslateRequest(text, sourceLang, targetLang, "text")));
			
			var deserialized = JsonSerializer.Deserialize<TranslateResponse>(response);
			var result = new List<string>();
			foreach (var item in deserialized.data.translations) {
				result.Add(item.translatedText);
			}
			return result;
		}
		public List<string> Translate(string targetLang, string text) {
			var sourceLang = DetectLanguage(text);
			if (sourceLang == targetLang) {
				return new List<string>() { text };
			}
			var response = WClient.UploadString(@$"https://translation.googleapis.com/language/translate/v2?key={ApiKey}", JsonSerializer.Serialize(new TranslateRequest(text, sourceLang, targetLang, "text")));
			var deserialized = JsonSerializer.Deserialize<TranslateResponse>(response);
			var result = new List<string>();
			foreach (var item in deserialized.data.translations) {
				result.Add(item.translatedText);
			}
			return result;
		}
		public void Dispose() {
			WClient?.Dispose();
		}
		private readonly string ApiKey;
		private readonly WebClient WClient = null;
	}
}
