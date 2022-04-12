package analysis

import (
	"reflect"
	"testing"
)

func TestPackageNode_Checksum(t *testing.T) {
	type fields struct {
		Name        string
		Path        string
		Size        int
		Sha1        SHA1
		Hash        HASH
		SubPackages PackageNodeList
	}
	tests := []struct {
		name   string
		fields fields
		want   PackageChecksum
	}{
		{
			name: "Empty",
			fields: fields{
				"",
				"",
				0,
				SHA1{},
				HASH{},
				nil,
			},
			want: PackageChecksum{},
		},
		{
			name: "Full",
			fields: fields{
				"",
				"",
				0xFFFFFFFF,
				SHA1{},
				HASH{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
					0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
					0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
					0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
				},
				nil,
			},
			want: PackageChecksum{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
				0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
				0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
				0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
				0xFF, 0xFF, 0xFF, 0xFF,
			},
		},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			node := PackageNode{
				Name:        tt.fields.Name,
				Path:        tt.fields.Path,
				Size:        tt.fields.Size,
				Sha1:        tt.fields.Sha1,
				Hash:        tt.fields.Hash,
				SubPackages: tt.fields.SubPackages,
			}
			if got := node.Checksum(); !reflect.DeepEqual(got, tt.want) {
				t.Errorf("PackageNode.Checksum() = %v, want %v", got, tt.want)
			}
		})
	}
}

func TestPackageNode_IsInCycle(t *testing.T) {
	noCycle := NewPackageGraph()
	c := noCycle.Insert("c", "a/b/c", 3, SHA1{0xC}, HASH{0xC})
	b := noCycle.Insert("b", "a/b/", 2, SHA1{0xB}, HASH{0xB}, c)
	a := noCycle.Insert("a", "a", 1, SHA1{0xA}, HASH{0xA}, b)

	cycle := NewPackageGraph()
	omega := cycle.Insert("omega", "a/b/c/z", 26, SHA1{26}, HASH{26})
	gamma := cycle.Insert("gamma", "z/a/b/c", 3, SHA1{3}, HASH{3}, omega)
	beta := cycle.Insert("beta", "c/z/a/b", 2, SHA1{2}, HASH{2}, gamma)
	alpha := cycle.Insert("alpha", "b/c/z/a", 1, SHA1{1}, HASH{1}, beta)
	omega.SubPackages.Add(alpha)
	moon := cycle.Insert("moon", "star/planet/moon", 4, SHA1{0x00}, HASH{0x00})
	planet := cycle.Insert("planet", "star/planet", 8, SHA1{0x0F}, HASH{0x0F}, moon)
	star := cycle.Insert("star", "star", 12, SHA1{0xF0}, HASH{0xF0}, planet, alpha)

	simple := NewPackageGraph()
	r := simple.Insert("r.zip", "r/r.zip", 4, SHA1{4}, HASH{4})
	r.SubPackages.Add(r)

	tests := []struct {
		name string
		node *PackageNode
		want bool
	}{
		{
			name: "no cycle",
			node: a,
			want: false,
		},
		{
			name: "cycle",
			node: alpha,
			want: true,
		},
		{
			name: "cycle under root, but not including root",
			node: star,
			want: false,
		},
		{
			name: "simple cycle",
			node: r,
			want: true,
		},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			if got := tt.node.IsInCycle(); got != tt.want {
				t.Errorf("PackageNode.IsInCycle() = %v, want %v", got, tt.want)
			}
		})
	}
}
