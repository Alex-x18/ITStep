using System.Collections.Generic;

namespace MonefyConsole {
	interface IMemoralbe<T> {
		string Filename { get; }
		IReadOnlyList<T> Data { get; }
		void LoadData();
		void UpdateData();
		void ReleaseData();
	}
}
