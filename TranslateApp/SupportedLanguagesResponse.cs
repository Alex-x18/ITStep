namespace TranslateApp {
	public class SupportedLanguagesResponse {
		public class Data {
			public class Language {
				public string language { get; set; }
			}
			public Language[] languages { get; set; }
		}
		public Data data { get; set; }
	}
}