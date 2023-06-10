"use strict";
var __decorate = (this && this.__decorate) || function (decorators, target, key, desc) {
    var c = arguments.length, r = c < 3 ? target : desc === null ? desc = Object.getOwnPropertyDescriptor(target, key) : desc, d;
    if (typeof Reflect === "object" && typeof Reflect.decorate === "function") r = Reflect.decorate(decorators, target, key, desc);
    else for (var i = decorators.length - 1; i >= 0; i--) if (d = decorators[i]) r = (c < 3 ? d(r) : c > 3 ? d(target, key, r) : d(target, key)) || r;
    return c > 3 && r && Object.defineProperty(target, key, r), r;
};
var __metadata = (this && this.__metadata) || function (k, v) {
    if (typeof Reflect === "object" && typeof Reflect.metadata === "function") return Reflect.metadata(k, v);
};
Object.defineProperty(exports, "__esModule", { value: true });
exports.MotifiInst = void 0;
var typeorm_1 = require("typeorm");
var motif_decription_entity_1 = require("./motif_decription.entity");
var locus_entity_1 = require("./locus.entity");
var MotifiInst = /** @class */ (function () {
    function MotifiInst() {
    }
    __decorate([
        (0, typeorm_1.PrimaryColumn)(),
        __metadata("design:type", Number)
    ], MotifiInst.prototype, "start", void 0);
    __decorate([
        (0, typeorm_1.PrimaryColumn)(),
        __metadata("design:type", Number)
    ], MotifiInst.prototype, "end", void 0);
    __decorate([
        (0, typeorm_1.Column)(),
        __metadata("design:type", Number)
    ], MotifiInst.prototype, "ForR", void 0);
    __decorate([
        (0, typeorm_1.ManyToOne)(function () { return motif_decription_entity_1.MotifDis; }, function (motifDis) { return motifDis.model; }),
        __metadata("design:type", motif_decription_entity_1.MotifDis)
    ], MotifiInst.prototype, "model_id", void 0);
    __decorate([
        (0, typeorm_1.OneToOne)(function () { return locus_entity_1.Locus; }),
        (0, typeorm_1.JoinColumn)(),
        __metadata("design:type", locus_entity_1.Locus)
    ], MotifiInst.prototype, "locus_id", void 0);
    MotifiInst = __decorate([
        (0, typeorm_1.Entity)()
    ], MotifiInst);
    return MotifiInst;
}());
exports.MotifiInst = MotifiInst;
//# sourceMappingURL=motif_instance.entity.js.map