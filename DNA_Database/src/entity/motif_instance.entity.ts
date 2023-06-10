import { BaseEntity, Column, ColumnTypeUndefinedError, Entity, JoinColumn, JoinTable, ManyToMany, OneToOne, ManyToOne,OneToMany, PrimaryColumn, PrimaryGeneratedColumn } from "typeorm";

import { MotifDis } from "./motif_decription.entity";
import { Locus } from "./locus.entity";
@Entity()
export class MotifiInst {
    @PrimaryColumn()
    start: number

    @PrimaryColumn()
    end: number

    @Column()
    ForR: number

    @ManyToOne(()=>MotifDis, (motifDis) => motifDis.model )
    model_id: MotifDis

    @OneToOne(()=>Locus)
    @JoinColumn()
    locus_id: Locus

    


}