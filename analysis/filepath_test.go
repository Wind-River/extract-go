package analysis

import (
	"testing"
)

func Test_isFailureExpected(t *testing.T) {
	type args struct {
		filePath string
		enclosed bool
		sha1Hex  string
	}
	tests := []struct {
		name    string
		args    args
		want    float64
		wantErr bool
	}{
		{
			name: "enclosed test hufts",
			args: args{
				"/tmp/bytebox/1d398dac6a7920a7de6e2685fe472a840eb2ce6e/gzip-1.4/tests/hufts-segv.gz",
				true,
				"1d398dac6a7920a7de6e2685fe472a840eb2ce6e",
			},
			want:    1.0,
			wantErr: false,
		},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			got, err := isFailureExpected(tt.args.filePath, tt.args.enclosed, tt.args.sha1Hex)
			if (err != nil) != tt.wantErr {
				t.Errorf("isFailureExpected() error = %v, wantErr %v", err, tt.wantErr)
				return
			}
			if got != tt.want {
				t.Errorf("isFailureExpected() = %v, want %v", got, tt.want)
			}
		})
	}
}

func TestIsFailureExpected(t *testing.T) {
	type args struct {
		filePath string
		enclosed bool
	}
	tests := []struct {
		name    string
		args    args
		want    float64
		wantErr bool
	}{
		{
			name: "enclosed test hufts",
			args: args{
				"../test/src/1d398dac6a7920a7de6e2685fe472a840eb2ce6e/gzip-1.4/tests/hufts-segv.gz",
				true,
			},
			want:    1.0,
			wantErr: false,
		},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			got, err := IsFailureExpected(tt.args.filePath, tt.args.enclosed)
			if (err != nil) != tt.wantErr {
				t.Errorf("IsFailureExpected() error = %v, wantErr %v", err, tt.wantErr)
				return
			}
			if got != tt.want {
				t.Errorf("IsFailureExpected() = %v, want %v", got, tt.want)
			}
		})
	}
}
